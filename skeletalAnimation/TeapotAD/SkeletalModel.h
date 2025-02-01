#ifndef SKELETALMODEL_H
#define SKELETALMODEL_H

#include "gl_core_4_3.hpp"
#include "glm\glm.hpp"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla
#include <vector>
#include <map>
#include "Drawable.h"
#include "Math3D.h"
#include "glslprogram.h"

struct VertexStruct 
{
	glm::vec3 position; // Posizione del vertice
	glm::vec3 normal; // Normale del vertice
	glm::vec2 uvs; // Coordinate UV del vertice
};

struct VertexBoneData
{
	unsigned int IDs[4]; //Un array di 4 ID di ossa che influenzano un singolo vertice
	float Weights[4]; //Un array dei pesi di influenza per ciascuna ossa

	VertexBoneData()
	{
		// Inizializza gli array a zero.
		Reset();
	}

	void Reset()
	{
		memset(IDs, 0, 4 * sizeof(IDs[0]));
		memset(Weights, 0, 4 * sizeof(Weights[0]));
	}

	void AddBoneData(unsigned int BoneID, float Weight)
	{
		for (unsigned int i = 0; i < 4; i++) {

			// Verifica se ci sono valori vuoti nei pesi
			if (Weights[i] == 0.0) {
				// Aggiungi l'ID dell'osso 
				IDs[i] = BoneID;

				// Imposta il peso di influenza del vertice per questo ID di osso
				Weights[i] = Weight;
				return;
			}
		}
		// Non dovrebbe mai arrivare qui - troppe ossa per lo spazio disponibile
		assert(0);
	}
};

// Memorizza le informazioni delle ossa
struct BoneInfo
{
	Matrix4f FinalTransformation; // Trasformazione finale da applicare ai vertici  
	Matrix4f BoneOffset; // Offset iniziale dallo spazio locale a quello dell'osso

	BoneInfo()
	{
		BoneOffset.SetZero();
		FinalTransformation.SetZero();
	}
};

// Un'entrata mesh per ciascuna mesh letta dalla scena Assimp. Un modello di solito è composto da una raccolta di queste.
#define INVALID_MATERIAL 0xFFFFFFFF
struct MeshEntry {


	unsigned int BaseVertex; //Numero totale degli indici della mesh.  
	unsigned int BaseIndex; //L'indice base di questa mesh nell'array dei vertici per l'intero modello.
	unsigned int NumIndices; //L'indice base di questa mesh nell'array degli indici per l'intero modello.
	unsigned int MaterialIndex; 

	MeshEntry()
	{

		NumIndices = 0; 
		BaseVertex = 0;  
		BaseIndex = 0; 
		MaterialIndex = INVALID_MATERIAL;
	}

	~MeshEntry() {}
};

class SkeletalModel : public Drawable
{
public:

	SkeletalModel(GLSLProgram* shaderProgIn); //Costruttore

	~SkeletalModel(); //Distruttore

	void LoadMesh(const std::string& Filename); //Carica una mesh animata da un percorso di file dato

	void BoneTransform(float TimeInSeconds, std::vector<Matrix4f>& Transforms); //!< Traversa la gerarchia della scena e ottiene la matrice di trasformazione per ciascuna ossa dato il tempo. 

	void SetBoneTransform(unsigned int Index, const Matrix4f& Transform); //Inserisce una trasformazione dell'osso nell'array dei uniformi all'indice dato.

	void render() const override; //Renderizza ogni mesh nel modello.

private:
	
	void LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones); //Carica i dati delle ossa da una mesh data.
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim); //Calcola il quaternione interpolato tra due keyframe 
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim); //Calcola il vettore di scaling interpolato tra due keyframe
	void CalcInterpolatedTranslation(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim); //Calcola il vettore di traslazione interpolato tra due keyframe

	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim); //Trova una keyframe di rotazione dato il tempo corrente dell'animazione 
	unsigned int FindScale(float AnimationTime, const aiNodeAnim* pNodeAnim); // Trova una keyframe di scaling dato il tempo corrente dell'animazione.
	unsigned int FindTranslation(float AnimationTime, const aiNodeAnim* pNodeAnim); //Trova una keyframe di traslazione dato il tempo corrente dell'animazione. 

	void ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform); //Funzione ricorsiva che attraversa la gerarchia dei nodi della scena e combina le trasformazioni delle matrici. 


	void InitFromScene(const aiScene* pScene, const std::string& Filename); //Prepara il modello per il rendering.
	void InitMesh(unsigned int index, const aiMesh* paiMesh, std::vector<VertexStruct>& Vertices, 
		std::vector<GLuint>& Indices, std::vector<VertexBoneData>& Bones); //Ottiene i dati della mesh dal dato mesh Assimp. 

	void Clear(); //Elimina l'oggetto dell'array dei vertici. 

	GLSLProgram* m_pShaderProg;

	GLuint m_VAO; // Vertex array object. 
	GLuint vbo; // Vertex buffer object. 
	GLuint ebo; // Indices buffer object. 
	GLuint boneBo; // Bone data buffer object. 

	const aiScene* pScene; //L'oggetto aiScene di Assimp.

	Assimp::Importer Importer; //Oggetto importatore di Assimp per leggere i file nell'oggetto aiScene. 

	unsigned int m_NumBones; //Numero totale di ossa nel modello. 

	std::map<std::string, unsigned int> m_BoneMapping; //Mappa dei nomi delle ossa agli ID

	std::vector<BoneInfo> m_BoneInfo; //Array contenente le informazioni delle ossa come matrice di offset e trasformazione finale.
	
	Matrix4f GlobalTransformation; //Trasformazione del nodo radice 
	Matrix4f m_GlobalInverseTransform; 

	std::vector<MeshEntry> m_Entries; //Array delle entrate delle mesh
};


#endif