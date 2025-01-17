#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords; //coordinate di texture del vertice

out vec3 FragPos; //posizione del vertice
out vec3 Normal;  //normale del vertice
out vec2 TexCoords;

//variabili uniform

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); // posizione del vertice
    Normal = mat3(transpose(inverse(model))) * aNormal; //normale del vertice trasformata nello spazio del mondo
    TexCoords = aTexCoords; //cordinate texture passate al fs
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}