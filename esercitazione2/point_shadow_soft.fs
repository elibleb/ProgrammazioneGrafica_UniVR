#version 330 core

out vec4 FragColor;


in VS_OUT {
    vec3 FragPos;   //Posizione del frammento nello spazio mondo
    vec3 Normal;    //Normale interpolata nello spazio mondo
    vec2 TexCoords; //Coordinate di texture
} fs_in;

//uniform
uniform sampler2D diffuseTexture;   //La texture diffusa del materiale
uniform samplerCube depthMap;       // Cube map delle ombre (profondità)

uniform vec3 lightPos;      //Posizione della luce
uniform vec3 viewPos;       //Posizione della camera

uniform float far_plane;    // Piano lontano per il calcolo delle ombre
uniform bool shadows;       // Booleano per abilitare/disabilitare le ombre

// array di direzioni offset
//Contiene 20 direzioni di offset in un cubo tridimensionale
//Viene usato per campionare punti attorno alla posizione del frammento nella cube map delle ombre

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

//calcolo delle ombre
//Questa funzione determina se un frammento si trova in ombra e calcola un valore di ombra morbida
float ShadowCalculation(vec3 fragPos)
{
    //vettore che punta dalla posizione del frammento alla posizione della luce
    vec3 fragToLight = fragPos - lightPos;
    //distanza attuale tra il frammento e la luce
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15; // correzione per gestire artefatti

    //Numero di campioni e raggio
    int samples = 20;
    float viewDistance = length(viewPos - fragPos); //distanza tra la posizione della fotocamera e il frammento
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0; //Il raggio del disco dipende dalla distanza del frammento 
                                                                  //dalla camera
                                                        //Questo aumenta il raggio con la distanza, sfumando le ombre lontane


    for(int i = 0; i < samples; ++i)
    {
        //recupera profondità dalla cubemap delle ombre (depthMap) con l'offset
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r; 
        closestDepth *= far_plane;
        if(currentDepth - bias > closestDepth) //confronta la profondità del frammento con quella del campione
            shadow += 1.0; //incrementa il valore se è in ombra
    }
    shadow /= float(samples);

    return shadow; //output: restituisce un valore normalizzato tra 0.0 se illuminato e 1.0 se in ombra
}

void main()
{           
    // prende il colore dalla texture diffuseTexture utilizzando le coordinate della texture del frammento
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);

     // componente ambientale: luce costante che illumina tutto l'oggetto in modo uniforme
    vec3 ambient = 0.3 * lightColor;

    // componente diffusa: basata sull'angolo tra la normale e la direzione della luce
    vec3 lightDir = normalize(lightPos - fs_in.FragPos); // calcolata come vettore tra la posizione della luce e quella del frammento
    float diff = max(dot(lightDir, normal), 0.0); //componente diffusa
    vec3 diffuse = diff * lightColor; //colore diffuso finale

    //Componente speculare: aggiunge riflessi basati sulla direzione della vista e della luce
    vec3 viewDir = normalize(viewPos - fs_in.FragPos); //direzione verso la camera
    vec3 reflectDir = reflect(-lightDir, normal); //direzione di riflessione della luce rispetto alla normale
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir); //direzione a metà strada tra la direzione della luce e la direzione della vista
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0); //speculare: riflessione basata sull'angolo tra la luce riflessa
                                                         //e la direzione della vista
    vec3 specular = spec * lightColor; //colore speculare  

    // calcola l'ombra
    //Applica l’ombra morbida solo alle componenti diffusa e speculare
    //La componente ambientale non è influenzata dall’ombra
    float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0); //Output finale, il colore del frammento include
                                     //Illuminazione ambientale, diffusa e speculare
                                     //Ombre morbide applicate.
}

//Differenze rispetto a ombre rigide
//Grid Sampling Disk: applica sfumature attorno ai bordi delle ombre
//diskRadius: aumenta il raggio del campionamento per frammenti distanti, ombre lontane più sfumate
//Soft Shadows, ombre morbide: il valore di ombra è una media di campioni, invece di ombre non ombra
