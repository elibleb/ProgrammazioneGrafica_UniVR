#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;   // Posizione del frammento nello spazio mondo
    vec3 Normal;    // Normale interpolata nello spazio mondo
    vec2 TexCoords; // Coordinate della texture
} fs_in;

uniform sampler2D diffuseTexture;   // Texture del materiale
uniform samplerCube depthMap;       // Cube map delle ombre

uniform vec3 lightPos;            // Posizione della luce puntiforme
uniform vec3 viewPos;             // Posizione della camera

uniform float far_plane;          // Piano lontano per il calcolo delle ombre
uniform bool shadows;             // Booleano per abilitare/disabilitare le ombre

//calcola se un frammento è in ombra o no
float ShadowCalculation(vec3 fragPos)
{
    //vettore che punta dalla posizione del frammento alla posizione della luce
    vec3 fragToLight = fragPos - lightPos;
    //profondità salvata nella depth map per la direzione,rappresenta distanza tra la luce e l'oggetto proiettato dalla luce stessa
    float closestDepth = texture(depthMap, fragToLight).r;
    // è in range [0,1] quindi lo riportiamo al valore di depth originale
    closestDepth *= far_plane; // Converti in coordinate mondo
    //distanza attuale tra il frammento e la luce
    float currentDepth = length(fragToLight);

    float bias = 0.05; // correzione per gestire artefatti
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0; // 1.0 se è in ombra, 0.0 se non lo è          
        
    return shadow;
}

void main()
{   
    // prende il colore dalla texture diffuseTexture utilizzando le coordinate della texture del frammento
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    
    vec3 normal = normalize(fs_in.Normal); //normale
    vec3 lightColor = vec3(0.3); //colore, bianco molto attenuato

    // componente ambientale: luce costante che illumina tutto l'oggetto in modo uniforme
    vec3 ambient = 0.3 * lightColor;

    // componente diffusa: basata sull'angolo tra la normale e la direzione della luce
    vec3 lightDir = normalize(lightPos - fs_in.FragPos); // calcolata come vettore tra la posizione della luce e quella del frammento
    float diff = max(dot(lightDir, normal), 0.0); //componenete diffusa
    vec3 diffuse = diff * lightColor; //colore diffuso finale

    // componente speculare: aggiunge riflessi basati sulla direzione della vista e della luce
    vec3 viewDir = normalize(viewPos - fs_in.FragPos); //direzione verso la camera
    vec3 reflectDir = reflect(-lightDir, normal); //direzione di riflessione della luce rispetto alla normale
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir); //direzione a metà strada tra la direzione della luce e la direzione della vista
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0); //speculare: riflessione basata sull'angolo tra la luce riflessa
                                                         //e la direzione della vista
    vec3 specular = spec * lightColor; //colore speculare 

    // calcola l'ombra:
    /*La funzione ShadowCalculation determina se il frammento è in ombra confrontando:
        currentDepth: Distanza reale tra il frammento e la luce
        closestDepth: Valore salvato nella depth map (proiettato dalla luce)
      Se il frammento è più lontano della profondità salvata, è in ombra.*/

    //shadows attivo: funzione ShadowCalculation per determinare se il frammento è in ombra o meno
    //Se shadows è disabilitato restituisce 0.0, non ci sono ombre
    float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;

    // Illuminazione totale con ombre applicate
    //Output: calcola la luce totale, riducendo la luce diffusa e speculare se il frammento è in ombra
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}