#version 330 core

out vec4 FragColor; // variabile di output di tipo vec4, rappresenta il colore finale del pixel. Il tipo vec4 è formato da 4 
                    //componenti: rosso R, verde G, blu B e alfa A

in vec3 ourColor; //variabile di input che proviene dal vertex shader. Contiene il colore interpolato tra i vari vertici del
//triangolo.

void main() //funzione principale, eseguita per ogni frammento (pixel) da colorare
{
    FragColor = vec4(ourColor, 1.0f); //ourColor, colore interpolato tra i vertici usato per settare il colore del frammento
    //aggiungendo un valore di opacità (alfa) 1.0f, il colore sarà completamente opaco
}