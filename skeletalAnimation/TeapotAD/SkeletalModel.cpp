#include "SkeletalModel.h"

//si occupa di gestire l'importazione, la trasformazione e il rendering di modelli 3D scheletrici animati. Usa Assimp per caricare
//i modelli, gestisce la struttura che tiene traccia delle ossa e delle animazioni, interpolando le trasformazioni tra i keyframe
//di animazione per ogni osso e applicando queste trasformazioni durante il rendering.


SkeletalModel::SkeletalModel(GLSLProgram* shaderProgIn)
{
	m_VAO = 0;

	pScene = NULL;

	// Inizializza il numero totale di ossa a 0
	m_NumBones = 0;

	// Ottieni il puntatore al programma shader da utilizzare per il rendering 
	m_pShaderProg = shaderProgIn;

}

SkeletalModel::~SkeletalModel()
{
	Clear();
}

void SkeletalModel::Clear()
{
	if (m_VAO != 0) {
		gl::DeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}

void SkeletalModel::LoadMesh(const std::string& Filename)
{
	// Rilascia la mesh precedentemente caricata (se esiste)
	Clear();
	// crea il VAO
	gl::GenVertexArrays(1, &m_VAO);
	gl::BindVertexArray(m_VAO);
		// Genera i buffer per gli attributi dei vertici
	gl::GenBuffers(1, &vbo);
	gl::GenBuffers(1, &ebo);
	gl::GenBuffers(1, &boneBo);
		pScene = Importer.ReadFile(Filename.c_str(), 
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType | 
		aiProcess_Triangulate | 
		aiProcess_GenSmoothNormals | 
		aiProcess_FlipUVs |
		aiProcess_LimitBoneWeights);
	if (pScene) {
		m_GlobalInverseTransform = pScene->mRootNode->mTransformation;
		m_GlobalInverseTransform.Inverse();
		InitFromScene(pScene, Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}
	gl::BindVertexArray(0);
}

void SkeletalModel::InitFromScene(const aiScene* pScene, const std::string& Filename)
{	
	m_Entries.resize(pScene->mNumMeshes);
	//m_Textures.resize(pScene->mNumMaterials);

	// Prepara i vettori per gli attributi dei vertici e gli indici
	std::vector<VertexStruct> vertices; 
	std::vector<VertexBoneData> bones;
	std::vector<unsigned int> Indices;

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;

		// Indici totali della mesh. 
		m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;

		// Imposta il vertice base di questa mesh (vertice iniziale per questa mesh nell'array di vertici) al numero totale corrente di vertici
 		m_Entries[i].BaseVertex = NumVertices;

		// Imposta l'indice base di questa mesh (indice iniziale per questa mesh nell'array degli indici) al numero totale corrente di indici 
		m_Entries[i].BaseIndex = NumIndices;

		// Incrementa il numero totale di vertici e indici 
		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices += m_Entries[i].NumIndices;
	}

	// Riserva spazio nei vettori per gli attributi dei vertici e gli indici
	vertices.reserve(NumVertices); 
	bones.resize(NumVertices);
	Indices.reserve(NumIndices);

	// Inizializza le mesh nella scena una per una
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh, vertices, Indices, bones);
	}

	// Genera e popola i buffer con gli attributi dei vertici e gli indici
	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::BufferData(gl::ARRAY_BUFFER, vertices.size() * sizeof(VertexStruct), &vertices[0],
		gl::STATIC_DRAW);

	//  Posizioni dei vertici
	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, sizeof(VertexStruct), (GLvoid*)0);

	// Normali dei vertici
	gl::EnableVertexAttribArray(1);
	gl::VertexAttribPointer(1, 3, gl::FLOAT, FALSE, sizeof(VertexStruct), (GLvoid*)offsetof(VertexStruct, normal));

	//// Coordinate UV dei vertici
	//gl::EnableVertexAttribArray(2);
	//gl::VertexAttribPointer(2, 2, gl::FLOAT, FALSE, sizeof(VertexStruct), (GLvoid*)offsetof(VertexStruct, uvs));


	// Collega il buffer dei dati delle ossa
	gl::BindBuffer(gl::ARRAY_BUFFER, boneBo);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], gl::STATIC_DRAW);

	gl::EnableVertexAttribArray(2);
	gl::VertexAttribIPointer(2, 4, gl::INT, sizeof(VertexBoneData), (const GLvoid*)0);

	gl::EnableVertexAttribArray(3);
	gl::VertexAttribPointer(3, 4, gl::FLOAT, FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, ebo);
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0],
		gl::STATIC_DRAW);

	vertices.clear();
	Indices.clear();
	bones.clear();
}

void SkeletalModel::InitMesh(unsigned int index, const aiMesh* paiMesh, std::vector<VertexStruct>& Vertices, std::vector<GLuint>& Indices, std::vector<VertexBoneData>& Bones)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Popola i vettori degli attributi dei vertici
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ?
			&(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		glm::vec3 glmTempPos = glm::vec3(pPos->x, pPos->y, pPos->z);
		glm::vec3 glmTempNormal = glm::vec3(pNormal->x, pNormal->y, pNormal->z);
		glm::vec2 glmTempUV = glm::vec2(pTexCoord->x, pTexCoord->y);


		VertexStruct v;
		v.position = glmTempPos;
		v.normal = glmTempNormal;
		v.uvs = glmTempUV;

		Vertices.push_back(v);
	}
	
	// Carica le ossa della mesh 
	if (paiMesh->HasBones()){
		LoadBones(index, paiMesh, Bones);

	}

	// Popola l'array degli indici
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}
	

}

void SkeletalModel::LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
{
	// Loop di tutte le ossa nella mesh di Assimp
	for (unsigned int i = 0; i < pMesh->mNumBones; i++) {
		unsigned int BoneIndex = 0;
		// Ottieni il nome dell'osso
		std::string BoneName(pMesh->mBones[i]->mName.data);
		// Se l'osso non è già nella mappa
		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
			// Imposta l'ID dell'osso come l'attuale numero totale di ossa
			BoneIndex = m_NumBones;
			// Incrementa il numero totale di ossa 
			m_NumBones++;
			// Aggiungi nuove informazioni sull'osso nel vettore delle ossa
			BoneInfo bi;
			m_BoneInfo.push_back(bi);
		}
		else {
			// L'ID dell'osso è già nella mappa 
			BoneIndex = m_BoneMapping[BoneName];
		}
		m_BoneMapping[BoneName] = BoneIndex;
		// Ottieni la matrice di offset che trasforma l'osso dallo spazio mesh allo spazio dell'osso
		m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;
		// Cicla su tutti i vertici influenzati da quest'osso (i pesi)
		for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
			// Ottieni un indice del vertice influenzato all'interno dell'array dei vertici
			unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			// Il valore di quanto quest'osso influenza il vertice 
			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			// Inserisci i dati dell'osso per un particolare ID di vertice. Un massimo di 4 ossa può influenzare lo stesso vertice 
			Bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
}

void SkeletalModel::BoneTransform(float TimeInSeconds, std::vector<Matrix4f>& Transforms)
{
	Matrix4f Identity;
	Identity.InitIdentity();

	float TicksPerSecond = pScene->mAnimations[0]->mTicksPerSecond;
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, pScene->mAnimations[0]->mDuration);

	ReadNodeHierarchy(AnimationTime, pScene->mRootNode, Identity);

	Transforms.resize(m_NumBones);

	// Popola il vettore delle trasformazioni con le nuove matrici di trasformazione per le ossa. 
	for (unsigned int i = 0; i < m_NumBones; i++) {
		Transforms[i] = m_BoneInfo[i].FinalTransformation;
	}

}

unsigned int SkeletalModel::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// Verifica se ci sono delle keyframe di rotazione. 
	assert(pNodeAnim->mNumRotationKeys > 0);

	// Trova la keyframe di rotazione appena prima del tempo corrente dell'animazione e restituisce l'indice. 
	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);

	return 0;
}

unsigned int SkeletalModel::FindScale(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	//Verifica se ci sono delle keyframe di scala
	assert(pNodeAnim->mNumScalingKeys > 0);

	// Trova la keyframe di scala appena prima del tempo corrente dell'animazione e restituisce l'indice
	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);

	return 0;
}

unsigned int SkeletalModel::FindTranslation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// Verifica se ci sono delle keyframe di traslazione
	assert(pNodeAnim->mNumPositionKeys > 0);

	// Trova la keyframe di traslazione appena prima del tempo corrente dell'animazione e restituisce l'indice. 
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);

	return 0;
}


void SkeletalModel::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// Abbiamo bisogno di almeno due valori per interpolare...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}
	//  Ottieni la keyframe di rotazione corrente. 
	unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);

	// Calcola la successiva keyframe di rotazione e verifica i limiti 
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);

	// Calcola il delta di tempo, cioè il tempo tra le due keyframe.
	float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;

	// Calcola il tempo trascorso all'interno del delta di tempo.  
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);

	// Ottieni i valori dei quaternioni per la keyframe corrente e successiva.
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;

	// Interpola tra di loro utilizzando il fattore
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);

	// Normalizza e imposta il riferimento 
	Out = Out.Normalize();
}

void SkeletalModel::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// Abbiamo bisogno di almeno due valori per interpolare...
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	//Ottieni la keyframe di scala corrente.
	unsigned int ScalingIndex = FindScale(AnimationTime, pNodeAnim);
	
	// Calcola la successiva keyframe di scala e verifica i limiti.
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	
	// Calcola il delta di tempo, cioè il tempo tra le due keyframe.
	float DeltaTime = pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime;
	
	// Calcola il fattore di interpolazione.
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);

	// Ottieni i valori di scala per la keyframe corrente e successiva.
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End= pNodeAnim->mScalingKeys[NextScalingIndex].mValue;

	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void SkeletalModel::CalcInterpolatedTranslation(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// Abbiamo bisogno di almeno due valori per interpolare...
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	// Ottieni la keyframe di traslazione corrente.
	unsigned int PositionIndex = FindTranslation(AnimationTime, pNodeAnim);

	// Calcola la successiva keyframe di traslazione e verifica i limiti.
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);

	// Calcola il delta di tempo, cioè il tempo tra le due keyframe.
	float DeltaTime = pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime;

	// Calcola il fattore di interpolazione.
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);

	// Ottieni i valori di traslazione per la keyframe corrente e successiva.
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;

	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void SkeletalModel::SetBoneTransform(unsigned int Index, const Matrix4f& Transform )
{	// Imposta la trasformazione per un determinato osso utilizzando l'indice
	assert(Index < 100);

	m_pShaderProg->setUniformIndex(Index, Transform);
}

void SkeletalModel::render() const
{
	gl::BindVertexArray(m_VAO);

	// Renderizza tutte le mesh del modello.
	for (unsigned int i = 0; i < m_Entries.size(); i++) {

		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		gl::DrawElementsBaseVertex(gl::TRIANGLES,
			m_Entries[i].NumIndices,
			gl::UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
			m_Entries[i].BaseVertex);
	}

	// Assicura che il VAO non venga cambiato dall'esterno
	gl::BindVertexArray(0);
}

void SkeletalModel::ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform)
{
	Matrix4f IdentityTest;
	IdentityTest.InitIdentity();

	// Ottieni il nome del nodo corrente.
	std::string NodeName(pNode->mName.data);

	// Usa la prima animazione 
	const aiAnimation* pAnimation = pScene->mAnimations[0];

	// Ottieni la trasformazione relativa al genitore del nodo. 
	Matrix4f NodeTransformation(pNode->mTransformation);

	const aiNodeAnim* pNodeAnim = NULL;

	// Trova il canale di animazione per il nodo corrente. 
	for (unsigned i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnimIndex = pAnimation->mChannels[i];

		// Se esiste un canale di animazione per il nodo, esegui l'interpolazione per rotazione, scala e traslazione. 
		if (std::string(pNodeAnimIndex->mNodeName.data) == NodeName) {
			pNodeAnim = pNodeAnimIndex;
		} 
	}

	if (pNodeAnim) {

		//// Interpola la scala e genera la matrice di trasformazione della scala
		//aiVector3D Scaling;
		//CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		//Matrix4f ScalingM;
		//ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

		// Interpola la rotazione e genera la matrice di trasformazione della rotazione
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

		// Interpola la traslazione e genera la matrice di trasformazione della traslazione
		aiVector3D Translation;
		CalcInterpolatedTranslation(Translation, AnimationTime, pNodeAnim);
		Matrix4f TranslationM;
		TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

		// Combina le trasformazioni precedenti
		NodeTransformation = TranslationM * RotationM;/* *ScalingM;*/
	}
	
	Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;
	
	// Applica la trasformazione finale all'osso indicizzato nell'array 
	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		unsigned int BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform  * GlobalTransformation *
			m_BoneInfo[BoneIndex].BoneOffset;
	}

	// Fa lo stesso per tutti i figli del nodo. 
	for (unsigned i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHierarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}
