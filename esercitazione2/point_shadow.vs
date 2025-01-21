#version 330 core

layout (location = 0) in vec3 aPos;         //Posizione del vertice
layout (location = 1) in vec3 aNormal;      //Normale del vertice 
layout (location = 2) in vec2 aTexCoords;   //Coordinate della texture

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;   //Posizione del frammento nello spazio mondo
    vec3 Normal;    // Normale nello spazio mondo
    vec2 TexCoords; // Coordinate della texture
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform bool reverse_normals;

//Output verso il Fragment Shader

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0)); //vs_out.FragPos trasforma la posizione del vertice nello spazio mondo usando 
                                                    //la matrice model
    
    //vs_out.Normal: Trasforma la normale nello spazio mondo
    if(reverse_normals) //Se reverse_normals è attivo, inverte la direzione della normale per disegnare l'interno di oggetti come una stanza
        vs_out.Normal = transpose(inverse(mat3(model))) * (-1.0 * aNormal); 
    else
        vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;  //passa le coordinate texture al fs
    gl_Position = projection * view * model * vec4(aPos, 1.0); //calcola la posizione finale del vertice applicando le matrici alla 
	//posizione originale aPos. Si avrà una nuova posizione
  
}