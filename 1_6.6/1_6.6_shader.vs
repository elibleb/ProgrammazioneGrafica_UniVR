#version 330 core

layout (location = 0) in vec3 aPos; //Definisce un attributo di input (aPos) che rappresenta la posizione del vertice (un vettore 3D). 
                                    //location = 0: questo attributo è associato alla posizione 0 nel buffer dei vertici
layout (location = 1) in vec3 aColor; //attributo di input (aColor) che rappresenta il colore del vertice (un vettore RGB). 
                                      //location = 1: questo attributo è associato alla posizione 1 nel buffer dei vertici

out vec3 ourColor; //l'output del vertex shader è un colore che verrà passato al fragment shader
                   //ourColor è un vettore 3D che contiene il colore del vertice.

void main()
{
    gl_Position = vec4(aPos, 1.0); //variabile predefinita gl_Position: usata per specificare la posizione finale del vertice 
                                   //nello spazio di proiezione. Il valore di aPos (posizione del vertice) viene convertito 
                                   //in un vec4 aggiungendo un valore 1.0 per la componente omogeneità

    ourColor = aColor; //Colore del vertice aColor viene passato all'output ourColor per essere utilizzato nel fragment shader
}
