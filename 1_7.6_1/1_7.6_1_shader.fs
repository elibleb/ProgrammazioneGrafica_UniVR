#version 330 core

out vec4 FragColor; //variabile in uscita, rappresenta il colore finale sullo schermo. 
//È di tipo vec4 RGBA,rosso, verde, blu e alfa (trasparenza)

//variabili in ingresso provenienti dal vs
in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
//texture1 variabile uniform di tipo sampler2D. "sampler" è un tipo di variabile che rappresenta una texture in OpenGL
//Le variabili uniform sono variabili globali che non cambiano durante il ciclo di rendering di un singolo oggetto.
//Usata per passare la texture caricata
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0f); //texture() funzione di GLSL che campiona la texture (texture1) usando 
	//le coordinate di texture passate come parametro TexCoord. Questo valore di colore è moltiplicato per ourColor (colore del vertice). 
	//La moltiplicazione permette di applicare un colore tintato o personalizzato sulla textur. 
	//Trasforma il colore del vertice (che è un vec3 con RGB) in un vec4 aggiungendo un valore alfa di 1.0 (opaco)
}