#version 330 core

out vec4 FragColor;

//proprietà del materiale dell'oggetto
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

//proprietà luce direzionale
struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//proprietà pointlight
struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//proprietà spotlight
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 4

//input dal vs, nello spazio mondo
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

//variabili uniform
  
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

// funzioni che calcolano l'illuminazione proveniente da diverse sorgenti di luci
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{ 
    // proprietà
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // calcolo dell'illuminazione
    // fase 1: calcolo luce direzionale
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // fase 2: calcolo point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // fase 3: calcolo luce spot
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
}


// calcola luce direzionale, dipende dalla sua direzioen
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    //diffusa: prodotto scalare tra la normale e la direzione della luce per determinare, dipende dall'angolo tra le due
    float diff = max(dot(normal, lightDir), 0.0);
    //speculare: calcola direzione della riflessione della luce
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //pow della lucentezza per simulare superfici + o - lucide
    //combina le componenti
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

//calcola pointlight, luce emette in tutte le direzioni da una posizione specifica
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    //uguale all'illuminazione diffusa direzionale
    float diff = max(dot(normal, lightDir), 0.0);
    //uguale all'illuminazione speculare direzionale
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //attenuazione: perde intensità all'aumenta la distanza dalla fonte di luce
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combina i componenti
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calcola il colore spotlight, luce con un cono di illuminazione definito dalla direzione e dalle angolazioni di taglio
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    //calcolo illuminazione diffusa
    float diff = max(dot(normal, lightDir), 0.0);
    //calcolo illuminazione speculare
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //attenuazione
    float distance = length(light.position - fragPos); //calcola la distanza tra la posizione della luce e il frammento
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    //intensità: in base alla direzione della luce rispetto al cono definito da cutOff e outerCutOff
    //Luce è più intensa quando si trova più vicino al centro del cono
    float theta = dot(lightDir, normalize(-light.direction)); //angolo tra la direzione della luce (lightDir) e la direzione del 
                                                             //cono (light.direction)
    float epsilon = light.cutOff - light.outerCutOff; //determinare come la luce si sfuma tra il cutoff interno e il cutoff esterno
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); //intensità/sfocatura luce cono
    // combina i risultati
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}