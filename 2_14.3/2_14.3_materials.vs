#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; //normale del vertice in input, indicano direzione della superficie in ciascun punto

out vec3 FragPos; //colore dell'illuminazione, verrà passata al fs
out vec3 Normal; //normale, verrà passata al fs

//variabili uniform

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); //posizione del frammento, converte la posizione del vertice dallo spazio del modello
    //allo spazio del mondo
    Normal = mat3(transpose(inverse(model))) * aNormal; //normale trasformata dallo spazio del modello allo spazio del mondo, 
    //per gestire correttamente la rotazione e la scala degli oggett
    
    gl_Position = projection * view * vec4(FragPos, 1.0); //posizione finale nello schermo
}