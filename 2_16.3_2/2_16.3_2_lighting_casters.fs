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
    vec3 lightDir = normalize(light.position - FragPos); //direzione della luce dalla posizione verso la posizione della sorgente luminosa
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

    //riduzione della luce in base alla distanza
    float distance = length(light.position - FragPos); //distanza tra la posizione del frammento e la sorgente luminosa
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient  *= attenuation;
    diffuse   *= attenuation;
    specular *= attenuation;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}