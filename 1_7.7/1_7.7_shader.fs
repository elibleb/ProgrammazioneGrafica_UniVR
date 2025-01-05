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
	// interpolazione lineare tra le due texture (50%/50%)
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5); //texture() funzione di GLSL che campiona la texture (texture1) usando 
	//le coordinate di texture passate come parametro TexCoord

	//In questo caso, texture(texture1, TexCoord) restituisce il colore (un vec4 che contiene le componenti RGBA) 
	//dalla texture1 in base alle coordinate di texture TexCoord passate dal vertex shader. Uguale per texture2

	//mix(a, b, t) funzione di interpolazione lineare che combina due valori a e b in base a un fattore di interpolazione t.
	//a è il colore campionato dalla texture1, b è il colore campionato dalla texture2.
	//t = 0.5: media 50%/50% tra i due colori delle texture. 
	//Se il valore di t fosse 0, il risultato sarebbe solo a (ovvero, solo la texture1), mentre se t fosse 1 sarebbe solo b (solo la texture2).
}