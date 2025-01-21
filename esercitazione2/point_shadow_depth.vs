
#version 330 core

layout (location = 0) in vec3 aPos; //Posizione dei vertici nello spazio oggetto

uniform mat4 model; //Matrice di trasformazione che converte i vertici dallo spazio oggetto allo spazio mondo

void main()
{
    gl_Position = model * vec4(aPos, 1.0); //Trasforma i vertici nello spazio mondo, fornisce la posizione trasformata
                                           //al Geometry Shader per ulteriori elaborazioni
}
