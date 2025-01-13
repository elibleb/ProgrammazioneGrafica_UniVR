#version 330 core

out vec4 FragColor; //colore finare

in vec3 LightingColor;  //colore in input calcolato dal vs

uniform vec3 objectColor; //variabile uniform, colore dell'oggetto

void main()
{
   //Gouraud shading, illuminazione calcolata ai vertici e poi interpolata tra i frammenti
   FragColor = vec4(LightingColor * objectColor, 1.0); //come la luce interagisce sulla superficie + colore dell'oggetto (riflessione
   //dell'oggetto stesso), il prodotto simula il comportamento della riflessione della luce sul cubo
}