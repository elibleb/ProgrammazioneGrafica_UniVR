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
uniform float percentage; //parametro che controlla il mix delle due texture nel fragment shader.

void main()
{
	// interpolazione lineare tra le due texture, all'inizio è 80%/20% perchè percentage è impostata a 0.2f
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), percentage);

	//Se percentage è 0.0, la funzione mix restituisce solo texture1 (container)
	//Se percentage è 1.0, la funzione mix restituisce solo texture2 (awesomeface)
	//Se percentage è un valore tra 0.0 e 1.0, la funzione mescola le due texture
}