#version 330 core

out vec4 FragColor; // variabile di output di tipo vec4, rappresenta il colore finale del pixel. Il tipo vec4 è formato da 4 
                    //componenti: rosso R, verde G, blu B e alfa A

in vec3 vPos; //input del fragment shader. Rappresenta la posizione del vertice che è stata passata dal vertex shader 
//è un vettore 3D, con componenti x, y, z

void main() //La funzione main che calcola il colore del frammento (pixel).

{
    FragColor = vec4(vPos, 1.0f); //Il colore finale del frammento viene determinato dalla posizione del vertice (vPos),
    //che viene utilizzato come colore. La variabile vPos è un vec3, ma il colore finale deve essere un vec4, si aggiunge quindi 
    //una componente alfa 1.0f, pienamente opaco. Quindi ogni componente di vPos viene usata per il colore (Rosso, Verde, Blu),
    //più 1.0f per l''opacità
}