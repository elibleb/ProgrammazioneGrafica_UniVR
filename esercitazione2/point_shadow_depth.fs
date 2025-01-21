#version 330 core

in vec4 FragPos; //posizione del frammento nello spazio mondo, ricevuta dal Geometry Shader

uniform vec3 lightPos; //posizione della luce puntiforme
uniform float far_plane; //distanza massima della luce, piano lontano

void main()
{
    //calcola la distanza tra il frammento e la luce
    float lightDistance = length(FragPos.xyz - lightPos);
    //normalizza questa distanza mappandola nell'intervallo [0, 1] e divide con far_plane
    lightDistance = lightDistance / far_plane;
    //output: assegna la distanza alla variabile gl_FragDepth
    gl_FragDepth = lightDistance;
    //valore salvato nella depth map della cube map per essere usato nel secondo passaggio del rendering
}