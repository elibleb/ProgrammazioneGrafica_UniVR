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

//in questo caso abbiamo due texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// interpolazione lineare tra le due texture (80%/20%)
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	//texture() funzione di GLSL che campiona le texture usando le coordinate di texture passate come parametro TexCoord

	//mix funzione di interpolazione lineare usata per mescolare i due colori delle texture
	//la percentuale è 0.2 (80% del colore da texture1 e 20% del colore da texture2)
}