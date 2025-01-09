#version 330 core
out vec4 FragColor; //variabile in uscita, rappresenta il colore finale sullo schermo

in vec2 TexCoord;//variabile in ingresso proveniente dal vs

// texture sampler
//variabili uniform di tipo sampler2D. "sampler" è un tipo di variabile che rappresenta una texture in OpenGL
//Le variabili uniform sono variabili globali che non cambiano durante il ciclo di rendering di un singolo oggetto.
//Usata per passare la texture caricata
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// interpolazione lineare tra le due texture (80%/20%)
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}