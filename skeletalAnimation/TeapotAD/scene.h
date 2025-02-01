#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <GLFW/glfw3.h>
#include "QuatCamera.h"

class Scene
{
public:
	Scene() : m_animate(true) { }
	
    /**
      Carica textures, inizializza shaders, etc.
      */
    virtual void initScene(QuatCamera camera) = 0;

    /**
      Viene chiamato prima di ogni frame. Si usa
      per aggiornare l'animazione.
      */
    virtual void update(long long f_startTime, float f_Interval) = 0;

    /**
      Disegna la scena.
      */
    virtual void render(QuatCamera camera) = 0;

    /**
      Chiamato quando lo schermo viene ridimensionato
      */
    virtual void resize(QuatCamera camera,int, int) = 0;
    
    void animate( bool value ) { m_animate = value; }
    bool animating() { return m_animate; }
    
protected:
	bool m_animate;
};

#endif // SCENE_H
