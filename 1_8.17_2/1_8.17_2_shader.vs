#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord; //variabile di tipo vec2, rappresenta le coordinate della texture per il vertice

//variabile che verrà passata al fragment shader

out vec2 TexCoord;

//variabile uniform, matrice di trasformazione, verrà applicata alla posizione del vertice

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0); //calcola la posizione finale del vertice applicando la matrice transform alla 
	//posizione originale aPos. Si avrà una nuova posizione

	TexCoord = vec2(aTexCoord.x, aTexCoord.y); //Le coordinate della texture vengono usate per mappare un'immagine 2D sui poligoni 3D
	//verranno passate al fragment shader
}