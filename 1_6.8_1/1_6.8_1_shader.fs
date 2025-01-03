#version 330 core

out vec4 FragColor; //output del fragment shader, determina il colore finale del pixel. 
                    //È un vettore di 4 componenti (RGBA), dove il quarto valore (A) rappresenta la trasparenza (opacità).

in vec3 ourColor; //input del fragment shader, che riceve il colore calcolato nel vertex shader. 
                  //Il valore di ourColor è un vettore di 3 componenti (egb), passato dal vertex shader.

void main()
{
    FragColor = vec4(ourColor, 1.0f); //il valore di ourColor (che è un vettore RGB) viene trasformato in un oggetto vec4 (RGBA),
                                      //dove la componente A è impostata su 1.0f (opaco).
                                      //Questo valore diventa il colore finale del frammento che viene visualizzato sullo schermo.
}

//Prende il colore calcolato nel vertex shader e lo usa per determinare il colore finale del pixel