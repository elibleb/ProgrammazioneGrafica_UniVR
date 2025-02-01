#version 330 core

//Geometry Shader utilizzato per manipolare o generare nuove primitive, come vertici o triangoli, a partire da un input

layout (triangles) in; //input: triangoli (triangles), ogni gruppo di 3 vertici rappresenta un triangolo
//ogni invocazione del gs riceverà un triangolo 

//output è una sequenza di vertici, massimo 18, per formare sei triangoli, tipo di primitiva in uscita "triangle_strip"
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6]; //contiene le 6 matrici, ognuna è una combinazione di matrice di vista e matrice di proiezione per una 
                                //delle 6 facce di una cube map, per ciascuna direzione della cube map

out vec4 FragPos; // output per EmitVertex

void main()
{
    //itera sulle 6 facce della cube map, ogni faccia rappresenta una direzione della luce
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // variabile che dice per quale faccia stiamo facendo il render
        for(int i = 0; i < 3; ++i) // per ciascun vertice del triangolo (ogni triangolo ha 3 vertici)
        {
            FragPos = gl_in[i].gl_Position; //prende la posizione passata dal vs contenuta in gl_in[i] e la salva in FragPos

            //per ciascun triangolo trasforma i vertici usando la matrice di proiezione e vista della faccia corrente
            //matrice trasforma la posizione del vertice dallo spazio mondo nello spazio luce per ogni faccia della cube map
            gl_Position = shadowMatrices[face] * FragPos;

            //Passa la posizione trasformata al fs
            EmitVertex(); //usa EmitVertex() per emettere i vertici trasformati
        }    
        EndPrimitive();
    }
} 
//output: genera un triangolo per ogni faccia della cube map
