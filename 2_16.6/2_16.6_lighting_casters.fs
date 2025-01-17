#version 330 core

out vec4 FragColor;

//proprietà del materiale dell'oggetto
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

//proprietà della luce
struct Light {
    vec3 position;
    vec3 direction;
    float cutOff; //angolo del cono di luce, oltre il quale la luce non viene più emessa
    float outerCutOff; // angolo esterno che definisce il limite oltre il quale la luce non ha effetto
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

//input dal vs, nello spazio mondo
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
  
uniform vec3 viewPos; //posizione telecamera
uniform Material material;
uniform Light light;

void main()
{ 
    // calcolo luce ambientale
    //prende il colore dalle coordinate della texture
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb; 
        
    // luce diffusa 
    //dipende dall'angolo tra la normale alla superficie (Normal) e la direzione della luce (lightDir)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos); //direzione della luce dalla posizione verso la posizione della sorgente luminos
    float diff = max(dot(norm, lightDir), 0.0); //prodotto scalare tra la normale e la direzione della luce per determinare 
                                                //quanto la luce colpisce il punto
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; //determina quanto la superficie è 
        //illuminata dalla luce direzionale, usa anche qui le coordinate della texture per il colore
        
    // luce speculare, simula i riflessi sulla superficie
    //Dipende dalla direzione della luce (lightDir), dalla normale e dalla posizione della camera (viewPos)
    vec3 viewDir = normalize(viewPos - FragPos); //calcola la direzione verso la telecamera
    vec3 reflectDir = reflect(-lightDir, norm); //calcola la direzione della riflessione della luce
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //pow della lucentezza per simulare superfici + o - lucide
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
    // spotlight con bordi attenuati
    float theta = dot(lightDir, normalize(-light.direction));  //angolo tra la direzione della luce (lightDir) e la direzione del 
                                                             //cono (light.direction)
    float epsilon = (light.cutOff - light.outerCutOff); //determinare come la luce si sfuma tra il cutoff interno e il cutoff esterno
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); //intensità/sfocatura luce cono
    diffuse  *= intensity;
    specular *= intensity;


    // attenuazione
    float distance = length(light.position - FragPos); //calcola la distanza tra la posizione della luce e il frammento
    //perdita di intensità della luce all'aumentare della distanza
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation;
    diffuse   *= attenuation;
    specular *= attenuation;   
            
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}