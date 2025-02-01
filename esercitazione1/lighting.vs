#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

//restituisce
out VS_OUT {
    vec3 FragPos; //posizione nello spazio mondo
    vec2 TexCoords; 
    vec3 Normal; //normale trasformata nello spazio mondo
    vec3 TangentViewPos; //posizione della fotocamera (vista) nello spazio tangent
    vec3 TangentFragPos;
    mat3 TBN; //matrice di trasformazione tangente, bitangente e normale
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;

void main()
{
    // calcola FragPos, TextCoords e Normal 
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));  //Trasformazione dei vertici dal sistema del modello allo spazio mondo
                                
    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;  //trasformazione della normale


    // configura la matrice TBN nello spazio mondo (poiché moltiplica per model)
    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0))); // Tangente trasformata nello spazio mondo
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0))); // Bitangente trasformata nello spazio mondo
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));    // Normale trasformata nello spazio mondo

    // inversa della matrice TBN, serve per trasformare i vettori dallo spazio mondo allo spazio tangent
    // usa il transpose essendo la matrice ortogonale
    vs_out.TBN = transpose(mat3(T, B, N));

    vs_out.TangentViewPos  = vs_out.TBN * viewPos;// Trasformazione della posizione della fotocamera nello spazio tangent
    vs_out.TangentFragPos  = vs_out.TBN * vs_out.FragPos; // Trasformazione della posizione del frammento nello spazio tangent

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
