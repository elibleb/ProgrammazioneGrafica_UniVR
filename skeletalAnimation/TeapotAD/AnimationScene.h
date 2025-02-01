#ifndef ANIMATIONSCENE_H
#define ANIMATIONSCENE_H

#include "gl_core_4_3.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <assimp\Importer.hpp>
#include <assimp/scene.h>           // Struttura dei dati di output
#include <assimp/postprocess.h>     // Post processing fla

#include "scene.h"
#include "glslprogram.h"
#include "SkeletalModel.h"

using glm::mat4;

class AnimationScene : public Scene
{
private:
    GLSLProgram* prog; // Shader program 

    int width, height;

	mat4 model; // Model matrix

	SkeletalModel* m_AnimatedModel; // skeletal model 

    void setMatrices(QuatCamera camera); //Imposta le matrici della fotocamera

    void compileAndLinkShader(); //Compila e collega lo shader

public:
	AnimationScene(); //Costruttore

	void setLightParams(QuatCamera camera); //Imposta i parametri dell'illuminazione

    void initScene(QuatCamera camera); // Inizializza la scena

    void update(long long f_startTime, float f_Interval); //aggiorna la scena

    void render(QuatCamera camera);	//Render scena

    void resize(QuatCamera camera, int, int); //Ridimensiona
};

#endif
