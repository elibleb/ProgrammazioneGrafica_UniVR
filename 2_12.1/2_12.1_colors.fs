#version 330 core

//fs cubo principale

out vec4 FragColor; //variabile in uscita, colore finale sullo schermo

//variabili uniform, non cambiano 
uniform vec3 objectColor; //colore del cubo, variabile tipo vec3, contiene i valori RGB
uniform vec3 lightColor; //colore della luce che illumina il cubo

void main()
{
	FragColor = vec4(lightColor * objectColor, 1.0); //simula effetto di illuminazione
}