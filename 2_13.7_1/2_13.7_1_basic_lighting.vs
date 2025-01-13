#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; //normale del vertice in input

out vec3 FragPos;//posizione finale del frammento in output
out vec3 Normal; //normale del frammento in output

//variabili uniform per le matrici

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); //posizione finale, applicata model, trasformazione
    Normal = mat3(transpose(inverse(model))) * aNormal;  //normale trasformata con matrice model
    
    //posizione finale del vertice in coordinate dello schermo
    gl_Position = projection * view * vec4(FragPos, 1.0);
}