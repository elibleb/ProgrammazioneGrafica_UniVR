#version 330 core

out vec4 FragColor;

// attributi passati dal vertex come input
in VS_OUT {
    vec3 FragPos; //Posizione nel mondo
    vec2 TexCoords; // Coordinate della texture
    vec3 Normal; // Normale del frammento
    vec3 TangentViewPos; // Posizione della vista (camera) nel tangent space
    vec3 TangentFragPos; // Posizione del frammento nel tangent space
    mat3 TBN; //Matrice per trasformare vettori
} fs_in;

struct Material {
    sampler2D diffuse; // componente diffuse
    sampler2D specular; // componente speculare
    sampler2D normal;
    float shininess; // shininess del materiale
};

// strutture utilizzate dal fragment shader per il lighting

// Directional light: la direzione della luce rimane sempre la stessa 
struct DirLight {
    vec3 direction; 
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Point light: emettono luce in tutte le direzioni da una posizione specifica
struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Spot light: emette luce solo nel raggio definito dallo spotlight
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

// Numero delle point light
#define NR_POINT_LIGHTS 4


// variabili uniform
uniform vec3 viewPos;
uniform DirLight dirLight;  
uniform PointLight pointLights[NR_POINT_LIGHTS]; // array delle point lights
uniform SpotLight spotLight;
uniform Material material;

// funzioni che calcolano l'illuminazione proveniente da diverse sorgenti di luci
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{ 
    // Tangent space
    //normaleMap texture che contiene vettori RGB che rappresentano le normali nel formato [0, 1]
    vec3 normal = texture(material.normal, fs_in.TexCoords).rgb;
    // trasforma il vettore della normale nell'intorno [-1, 1] nello spazio tangente
    normal = normalize(normal * 2.0 - 1.0);
    
    //normale passata dal vs (fs_in.Normal), che è già nello spazio mondo
    vec3 norm = normalize(fs_in.Normal);
 
    vec3 tanViewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos); //Direzione della vista trasformata nel tangent space
    vec3 viewDir = normalize(viewPos - fs_in.FragPos); //Direzione della vista nel world space


    // calcola il contributo di ogni luce e lo somma in result
    // Directional light
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // Point light: calcolo il contributo di ogni cubo
    for (int i = 0; i < NR_POINT_LIGHTS; i += 1) {
        result += CalcPointLight(pointLights[i], normal, fs_in.TangentFragPos, tanViewDir);
    }
    // Spot light
    result += CalcSpotLight(spotLight, norm, fs_in.FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
}


// calcolo del colore sulla Directional Light
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // negativa poiché va dal frammento verso la sorgente di luce
    vec3 lightDir = normalize(-light.direction); 
    // Diffuse shading 
    float diff = max(dot(normal, lightDir), 0.0);//prodotto scalare tra la normale del frammento e la direzione della luce
                                                 //determina quanto il frammento è esposto alla luce direzionale
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal); //direzione in cui la luce si riflette rispetto alla normale

    //ottiene coseno dell'angolo tra viewDir e reflectDir, determina quanto riflessiva è la superficie
    //eleva il valore del coseno a una potenza che dipende dalla lucentezza (shininess) del materiale
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combina i risultati
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    return (ambient + diffuse + specular);
}


// calcolo del colore sulla point light
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // direzione luce = posizione della luce x la matrice TBN nel tagent space - fragPos
    vec3 lightDir = normalize((fs_in.TBN * light.position) - fragPos);
    // Diffuse shading
    //prodotto scalare tra la normale del frammento (normal) e la direzione della luce (lightDir), max: valore sempre positivo
    float diff = max(dot(normal, lightDir), 0.0);  
    // Specular shading
    //direzione del riflesso della luce sulla superficie
    vec3 reflectDir = reflect(-lightDir, normal); //vettore negativo (-lightDir per ottenere il raggio riflesso rispetto alla normale
    //luce speculare
    //eleva il coseno alla shininess, aumenta concentrazione riflesso per superfici più lucide
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // distanza dalla sorgente di luce
    float distance = length((fs_in.TBN * light.position) - fragPos); //Trasforma la posizione della luce nel tangent space
    // formula per l'attenuazione, modella l'intensità della luce in base alla distanza
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combina i risultati
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    // moltiplica le componenti per l'attenuazione
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}


// calcola il colore per la spot light
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{                        
    //direzione dalla posizione del frammento (fragPos) alla posizione della luce (light.position)
    vec3 lightDir = normalize(light.position - fragPos); 
    // Diffuse shading 
    float diff = max(dot(normal, lightDir), 0.0); 
    // Specular shading 
    vec3 reflectDir = reflect(-lightDir, normal); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); 
    // distanza dalla sorgente di luce
    float distance = length(light.position - fragPos);
    // formula per l'attenuazione
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // theta per per determinare quanto il frammento si trova all'interno del cono di luce
    float theta = dot(lightDir, normalize(-light.direction));  //Usa un angolo interno (cutOff) e uno esterno (outerCutOff) 
                                                               //per modellare un cono di luce con bordi morbidi
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); //intensità cambia se fuori o dentro il cono
    // combina i risultati
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    // moltiplica le componenti per attenuazione ed intensità
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
