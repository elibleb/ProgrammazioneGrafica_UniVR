#version 330 core

layout (location = 0) in vec3 aPos;

//variabili uniform, matrici di trasformazione, verranno applicate alla posizione del vertice

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);//calcola la posizione finale del vertice applicando le matrici alla 
	//posizione originale aPos. Si avrà una nuova posizione
}