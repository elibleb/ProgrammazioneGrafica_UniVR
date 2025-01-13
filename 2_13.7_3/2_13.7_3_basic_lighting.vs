#version 330 core

layout (location = 0) in vec3 aPos; //posizione del vertice in input
layout (location = 1) in vec3 aNormal; //normale del vertice in input, indicano direzione della superficie in ciascun punto

out vec3 LightingColor; //colore dell'illuminazione, verrà passata al fs

//variabili uniform, non cambiano

uniform vec3 lightPos; // Posizione della luce
uniform vec3 viewPos; // Posizione della camera view
uniform vec3 lightColor; // Colore della luce

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); //posizione finale del vertice
    
    vec3 Position = vec3(model * vec4(aPos, 1.0)); //calcolo posizione globale del vertcice
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal; //calcolo normale globale

    //illuminazione calcolata per ogni vertice
    
    // illuminazione ambientale, luce arriva da tutte le parti in modo uniforme, non dipende dall'angolo di visione

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // illuminazione diffusa, luce dipende dall'angolo tra la luce e la normale, 
    //dipende da quanto la superficie è rivolta verso la luce

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0); //prodotto scalare, determina quanto la luce colpisce la superficie
    vec3 diffuse = diff * lightColor;
    
    // illuminazione speculare, luce riflessa, dipende dall'angolo di vista, simula riflessione luce sulla superficie

    float specularStrength = 1.0; // Forza dell'effetto speculare
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  // calcola direzione di riflessione
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // determina l'intensità del riflesso
    vec3 specular = specularStrength * spec * lightColor;      

    //Gouraud shading, illuminazione calcolata ai vertici e poi interpolata tra i frammenti
    //cambia in moodo graduale (viene smussata) tra i vertici (Il phong calcola illuminazione su ogni pixel)
    LightingColor = ambient + diffuse + specular; // combina tutte le componenti di illuminazione
}