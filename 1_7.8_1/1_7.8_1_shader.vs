#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord; //variabile di tipo vec2, rappresenta le coordinate della texture per il vertice

//variabili che verranno passate al fragment shader

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y); //Le coordinate della texture vengono usate per mappare un'immagine 2D sui poligoni 3D
	//verranno passate al fragment shader
}