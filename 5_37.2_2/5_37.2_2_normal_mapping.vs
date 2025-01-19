#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));//Trasformazione dei vertici dal sistema del modello al sistema di 
                                                   //coordinate mondiali
    vs_out.TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model))); //trasformare i vettori dallo spazio del modello a quello del mondo
    vec3 T = normalize(normalMatrix * aTangent); //calcolo tangente e bitangente per ogni vertice
    vec3 N = normalize(normalMatrix * aNormal); //normale traformata spazio mondo
    T = normalize(T - dot(T, N) * N); //normale tangente ortogonalizzata rispetto alla normale vera
    vec3 B = cross(N, T);
    
    // matrice TBN per convertire i vettori di luce e vista dallo spazio mondo allo spazio tangente
    mat3 TBN = transpose(mat3(T, B, N));  
    
    //posizioni della luce, camera, e frammento vengono trasformate nello spazio tangente per il calcolo 
    //dell'illuminazione nel fs
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
        
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}