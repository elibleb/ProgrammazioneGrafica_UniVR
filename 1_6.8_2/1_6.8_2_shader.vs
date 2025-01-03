#version 330 core

layout (location = 0) in vec3 aPos; //si aspetta un attributo di input aPos (posizione del vertice) come un vettore di 3 float(vec3).
                                    //L'attributo viene passato tramite il VBO e viene associato alla location 0
layout (location = 1) in vec3 aColor; //Uguale a sopra. Attributo di input aColor (colore del vertice) passato come
                                      //un vettore di 3 float (vec3) nella location 1

out vec3 ourColor; //variabile output del vertex shader. Valore verrà passato al fs per determinare il colore finale del pixel

uniform float offset; //variabile uniform, un valore che viene fornito nel codice C++ e che rimane costante per tutti i vertici
//durante il rendering. Viene utilizzato per applicare uno spostamento (traslazione) ai vertici

void main()
{
    gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0); //La posizione del vertice viene modificata nella coordinata x 
    //aggiungendo il valore di offset. Le altre coordinate (y e z) rimangono invariati. Il vertice viene traslato lungo l'asse
    //orizzontale (x) in base al valore di offset. gl_Position variabile tipo vec4 che indica la posizione finale del vertice
    //dopo l'esecuzione del vertex shader. 

    ourColor = aColor; //colore passato al fragment shader
}