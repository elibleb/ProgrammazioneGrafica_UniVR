
#include "AnimationScene.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;

#include "defines.h"

using glm::vec3;


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//definisce la scena dell'applicazione grafica, dove viene animato un modello scheletrico. Il modello viene caricato da un file
// e viene animato attraverso un sistema di ossa. L'illuminazione e le trasformazioni vengono gestite tramite shader.
//La scena viene continuamente aggiornata e renderizzata per simulare l'animazione in tempo reale.

/////////////////////////////////////////////////////////////////////////////////////////////
// Costruttore di default
/////////////////////////////////////////////////////////////////////////////////////////////
AnimationScene::AnimationScene()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Inizializza la scena
/////////////////////////////////////////////////////////////////////////////////////////////
void AnimationScene::initScene(QuatCamera camera)
{
	prog = new GLSLProgram();

    //Compila e collega lo shader 
	compileAndLinkShader();

	prog->initialiseBoneUniforms();

    gl::Enable(gl::DEPTH_TEST);

 	//Imposta i parametri di illuminazione
	setLightParams(camera);

	// Inizializza il modello scheletrico 
	m_AnimatedModel = new SkeletalModel(prog);
	
	// Carica il modello dal percorso dato. 
	m_AnimatedModel->LoadMesh("Assets/Minotaur@Jump.FBX");
	//m_AnimatedModel->LoadMesh("Assets/Capoeira.dae");
	//m_AnimatedModel->LoadMesh("Assets/Crouch To Stand.dae");
	
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Aggiorna
/////////////////////////////////////////////////////////////////////////////////////////////
void AnimationScene::update(long long f_StartTime, float f_Interval)
{
	// Vettore delle matrici di trasformazione delle ossa 
	std::vector<Matrix4f> Transforms;
	
	// Ottiene le matrici trasformate aggiornate dalla gerarchia delle ossa al tempo dato 
	m_AnimatedModel->BoneTransform(f_Interval, Transforms);

	// Passa ogni nuova trasformazione dell'osso nello shader
	for (unsigned int i = 0; i < Transforms.size(); i++) {
		m_AnimatedModel->SetBoneTransform(i, Transforms[i]);
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Imposta le variabili di illuminazione nello shader
/////////////////////////////////////////////////////////////////////////////////////////////
void AnimationScene::setLightParams(QuatCamera camera)
{
	vec3 worldLight = vec3(10.0f, 10.0f, 10.0f);

	prog->setUniform("lightIntensity", 0.5f, 0.5f, 0.5f);
	prog->setUniform("lightPos", worldLight);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Renderizza la scena
/////////////////////////////////////////////////////////////////////////////////////////////
void AnimationScene::render(QuatCamera camera)
{
    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Matrice del modello 
	model = mat4(1.0f);
	//model = glm::translate(glm::vec3(0.0, -20.0, 0.0));
	model = glm::scale(glm::vec3(0.2f));

	setMatrices(camera);

	 //Imposta le proprietà del materiale del Teapot nello shader e renderizza
	//prog->setUniform("Ka", vec3(0.225f, 0.125f, 0.0f));
	prog->setUniform("Ka", vec3(0.225f, 0.125f, 0.0f));
	prog->setUniform("Kd", vec3(1.0f, 0.6f, 0.0f));
	prog->setUniform("Ks", vec3(1.0f, 1.0f, 1.0f));
	prog->setUniform("specularShininess", 32.0f);

	m_AnimatedModel->render();
	
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Invia le matrici MVP alla GPU
/////////////////////////////////////////////////////////////////////////////////////////////
void AnimationScene::setMatrices(QuatCamera camera)
{
    mat4 mv = camera.view() * model;
    prog->setUniform("ModelViewMatrix", mv);

    prog->setUniform("MVP", camera.projection() * mv);

	// La matrice corretta per trasformare la normale è la trasposta dell'inverso della matrice M
	mat3 normMat = glm::transpose(glm::inverse(mat3(model)));

	prog->setUniform("M", model);
	prog->setUniform("NormalMatrix", normMat);
	prog->setUniform("V", camera.view() );
	prog->setUniform("P", camera.projection() );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Ridimensiona la finestra di visualizzazione
/////////////////////////////////////////////////////////////////////////////////////////////
void AnimationScene::resize(QuatCamera camera, int w, int h)
{
    gl::Viewport(0,0,w,h);
    width = w;
    height = h;
	camera.setAspectRatio((float)width/height);

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Compila e collega lo shader
/////////////////////////////////////////////////////////////////////////////////////////////
void AnimationScene::compileAndLinkShader()
{
   
	try {
    	prog->compileShader("Shaders/diffuse.vert");
    	prog->compileShader("Shaders/diffuse.frag");
    	prog->link();
    	prog->validate();
    	prog->use();
    } catch(GLSLProgramException & e) {
 		cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
