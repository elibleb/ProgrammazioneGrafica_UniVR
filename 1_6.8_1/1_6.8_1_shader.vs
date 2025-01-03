#version 330 core

layout (location = 0) in vec3 aPos; //si aspetta un attributo di input aPos (posizione del vertice) come un vettore di 3 float (vec3).
                                    //L'attributo viene passato dalla GPU tramite il VBO e viene associato alla location 0
layout (location = 1) in vec3 aColor; //Uguale a sopra. Attributo di input aColor (colore del vertice) passato dalla GPU come
                                      //un vettore di 3 float (vec3) nella location 1

out vec3 ourColor; //variabile output del vertex shader. Valore verrà passato al fs per determinare il colore finale del pixel

void main()
{
    gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0); //Si imposta la posizione del vertice (gl_Position variabile predefinita in OpenGL).
    //-aPos.y inverte la componente Y della posizione, il triangolo verrà disegnato con la parte superiore verso il basso. 
    ourColor = aColor; //copia il valore del colore del vertice nella variabile ourColor, sarà poi utilizzata nel fragment shader.
}