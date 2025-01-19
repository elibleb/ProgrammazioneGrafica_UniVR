#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;


void main()
{ 
    // ottiene la normale dalla normal map nell'intorno [0, 1]
    //normaleMap texture che contiene vettori RGB che rappresentano le normali nel formato [0, 1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // trasforma il vettore della normale nell'intorno [-1, 1] nello spazio tangente
    normal = normalize(normal * 2.0 - 1.0); 
      
    //ottiene il colore della superficie dalla diffuse map utilizzando le coordinate UV passate dal vs
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse: dipende dall'angolo tra la normale della superficie (modificata dalla normal map) e la direzione della luce
    //queste sono nel tanget space
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos); //direzione della luce rispetto al frammento
    float diff = max(dot(lightDir, normal), 0.0); //intensità dell'illuminazione diffusa in base all'orientamento 
                                                  //della superficie rispetto alla luce
    vec3 diffuse = diff * color; //color colore ottenuto dalla diffuse map
    // specular:  dipende dall'angolo tra la vista e la riflessione della luce, simula riflessi brillanti
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos); //direzione della fotocamera
                                                                           //rispetto alla posizione del frammento
    vec3 reflectDir = reflect(-lightDir, normal); // irezione del riflesso della luce sulla superficie
    vec3 halfwayDir = normalize(lightDir + viewDir); //Direzione "a metà strada"" tra la direzione della luce (lightDir)
                                                     //e la direzione della vista (viewDir)
    //intensità speculare in base all'angolo tra la normale e la direzione halfway
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0); 

    vec3 specular = vec3(0.2) * spec; // calcola il colore speculare

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}