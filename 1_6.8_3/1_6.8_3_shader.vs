#version 330 core

layout (location = 0) in vec3 aPos; //si aspetta un attributo di input aPos (posizione del vertice) come un vettore di 3 float(vec3).
                                    //L'attributo viene passato tramite il VBO e viene associato alla location 0
layout (location = 1) in vec3 aColor; //Uguale a sopra. Attributo di input aColor (colore del vertice) passato come
                                      //un vettore di 3 float (vec3) nella location 1

out vec3 vPos; //variabile output del vertex shader. Valore verrà passato al fs per determinare il colore finale del pixel

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); //gl_Position vettore vec4 che rappresenta la posizione finale del vertice.
    //la posizione del vertice è impostata come (aPos.x, aPos.y, aPos.z, 1.0), si aggiunge la componente omogenea 1.0, 
    //indica la posizione del vertice nello spazio 3D.
    vPos = aPos; //La posizione originale del vertice (aPos) viene passata come dato all'output vPos,
    //che sarà poi utilizzato dal fragment shader.
}