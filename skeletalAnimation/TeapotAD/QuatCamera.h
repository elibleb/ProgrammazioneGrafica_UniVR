#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class QuatCamera 
{
public:

	QuatCamera();  //costruttore

	const glm::vec3& position() const; //position getter method
	void setPosition(const glm::vec3& position); //position setter method

	float fieldOfView() const; //fieldOfView getter method
	void setFieldOfView(float fieldOfView); //fieldOfView setter method

	float aspectRatio() const; //aspectRatio getter method
	void setAspectRatio(float aspectRatio);  //aspectRatio setter method

	float nearPlane() const;  //nearPlane getter method
	float farPlane() const;   //farPlane getter method
	void setNearAndFarPlanes(float nearPlane, float farPlane); //nearPlane e farPLane setter method


	void rotate(const float yaw, const float pitch); //Rotazione camera
	void pan(const float x, const float y);  //Panoramica camera
	void roll(const float z); //Roll camera
	void zoom(const float z); //Zoom camera


	void updateView();  //aggiorna la camera

	void reset(void); //Reset della camera

	glm::mat4 view(); //Get View matrix

	glm::mat4 projection(); //Get Projection matrix
	

private:

	float _fieldOfView;
	float _nearPlane;
	float _farPlane;
	float _aspectRatio; 

	//Gli assi delle coordinate della telecamera
	glm::vec3 _xaxis;
	glm::vec3 _yaxis;
	glm::vec3 _zaxis;

	//Vettore di posizione della telecamera e Quaternoin per rappresentare l'orientamento della telecamera
	glm::vec3 _position;
	glm::quat _orientation;

	glm::mat4 _view;
	glm::mat4 _projection;
};