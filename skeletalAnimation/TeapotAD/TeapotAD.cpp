// APPLICATION ENTRY POINT 

#include "stdafx.h"
#include <iostream>

#include "gl_core_4_3.hpp"

#include <GLFW/glfw3.h>

#include "scene.h"

#include "AnimationScene.h"

#include "QuatCamera.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define MOVE_VELOCITY 0.01f
#define ROTATE_VELOCITY 0.001f

//codice principale: crea finestra che mostra la scena, interazione con l'utente utilizzando il mouse, ciclo principale che si occupa di
//aggiornare la scena e renderizzarla, sfrutta la telecamera.

//La finestra GLFW
GLFWwindow *window;

//La scena
Scene *scene;

//La camera
QuatCamera camera;

//Per tenere traccia della posizione del cursore
double lastCursorPositionX, lastCursorPositionY, cursorPositionX, cursorPositionY;

/////////////////////////////////////////////////////////////////////////////////////////////
//Funzione di callback per la pressione dei tasti, usata per attivare/disattivare l'animazione (attualmente non utilizzata)
// e per controllare se viene premuto 'R' per resettare la fotocamera
/////////////////////////////////////////////////////////////////////////////////////////////
static void key_callback(GLFWwindow* window, int key, int cancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		if (scene)
			scene->animate(!(scene->animating()));
	if (key == 'R' && action == GLFW_RELEASE)
			camera.reset();
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Funzione di callback quando la rotellina del mouse viene scrollata
/////////////////////////////////////////////////////////////////////////////////////////////
void scroll_callback(GLFWwindow *window, double x, double y)
{
			camera.zoom((float)y*0.5f);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Inizializzazione
/////////////////////////////////////////////////////////////////////////////////////////////
void initializeGL() {

    gl::ClearColor(0.5f,0.5f,0.5f,1.0f);
    
	lastCursorPositionX=0.0;
	lastCursorPositionY=0.0;
	cursorPositionX=0.0;
	cursorPositionY=0.0;
	

	// Crea la classe della scena e inizializzala per la fotocamera
	scene = new AnimationScene();
    scene->initScene(camera);

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Aggiornamento
/////////////////////////////////////////////////////////////////////////////////////////////
void update( float t ) 
{ 
	
	//Ottieni la posizione attuale del cursore
	glfwGetCursorPos(window,&cursorPositionX,&cursorPositionY);

	// Calcola quanto il cursore si è spostato
	float deltaX = (float)(lastCursorPositionX - cursorPositionX);
	float deltaY = (float)(lastCursorPositionY - cursorPositionY);

	//Controlla il pulsante sinistro del mouse (in un modo diverso dal callback)
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) )
	{
		//std::cout << "Left button \n";
		//Ruota la camera. Il valore 0.001f è un modificatore di velocità per rendere la velocità più sensibile

	//	std::cout <<"deltaX " << deltaX << " deltaY " << deltaY << "\n";

		camera.rotate(deltaX*ROTATE_VELOCITY, deltaY*ROTATE_VELOCITY);
		
	}
	
	//Controlla il pulsante destro del mouse (in un modo diverso dal callback)
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) )
	{
		//std::cout << "Right button \n";
		// Ruota la fotocamera. Il valore 0.01f è un modificatore di velocità per rendere la velocità più sensibile
		camera.pan(deltaX*MOVE_VELOCITY, deltaY*MOVE_VELOCITY);

	}
	//Per aggiustare il Roll con il pulsante centrale del mouse
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_MIDDLE) )
	{
	
		camera.roll(deltaX* ROTATE_VELOCITY);
		
	}
		
	// Memorizza la posizione attuale del cursore
	lastCursorPositionX = cursorPositionX;
	lastCursorPositionY = cursorPositionY;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Ciclo principale aggiorna la scena e la renderizza fino alla chiusura
/////////////////////////////////////////////////////////////////////////////////////////////
void mainLoop() {

	long long f_startTime = glfwGetTime();

	while( ! glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) ) {
		//GLUtils::checkForOpenGLError(__FILE__,__LINE__);

		double fCurrentTime = glfwGetTime();
		float fInterval = (fCurrentTime - (double)f_startTime);

		update(fInterval);

		scene->update(f_startTime, fCurrentTime);

		scene->render(camera);

		glfwSwapBuffers(window);
		glfwPollEvents();

		f_startTime = fCurrentTime;

	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Ridimensionamento
/////////////////////////////////////////////////////////////////////////////////////////////
void resizeGL(QuatCamera camera, int w, int h ) {
    scene->resize(camera,w,h);
}


/////////////////////////////////////////////////////////////////////////////////////////////
// main entry point
/////////////////////////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	// Inizializza GLFW
	if( !glfwInit() ) exit( EXIT_FAILURE );

	// Seleziona OpenGL 4.3 con un profilo core compatibile con le versioni successive.
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, TRUE);

	// Apre la finestra
	string title = "Skeletal Animation w/ Assimp";
	window = glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, title.c_str(), NULL, NULL );
	if( ! window ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	glfwMakeContextCurrent(window);

	//Callback per i tasti
	glfwSetKeyCallback(window,key_callback);

	//Callback per il mouse, non utilizzato al momento
	//glfwSetMouseButtonCallback(window,mouse_callback);

	//Scroll callback
	glfwSetScrollCallback(window,scroll_callback);//Set callback

	// Carica le funzioni OpenGL.
	gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();

	if (!didLoad) {

		glfwTerminate();
		exit(EXIT_FAILURE);
	}



	// Inizializzazione
	initializeGL();

	resizeGL(camera,WIN_WIDTH,WIN_HEIGHT);


	// Entra nel mainLoop
	mainLoop();

	// Chiudi la finestra e termina GLFW
	glfwTerminate();

	delete scene;

	// Esci dal programma
	exit( EXIT_SUCCESS );
}




