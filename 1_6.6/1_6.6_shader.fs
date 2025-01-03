#version 330 core

out vec4 FragColor; //FragColor è l'output, vettore di 4 componenti (RGBA). Rappresenta il colore finale del
                    //frammento (pixel) ed è il colore che verrà visualizzato sullo schermo

in vec3 ourColor; //input del fragment shader, viene passato dal vertex shader e rappresenta il colore del vertice
                  //che è un vettore 3D (RGB)

void main()
{
    FragColor = vec4(ourColor, 1.0f); //il colore del vertice (ourColor) viene trasformato in un vec4 (RGBA) dove la componente
                                      //alpha (trasparenza) è impostata a 1.0f, indicando che il colore è completamente opaco. 
                                      //Questo colore viene quindi assegnato a FragColor, che rappresenta il colore finale del pixel.
}
