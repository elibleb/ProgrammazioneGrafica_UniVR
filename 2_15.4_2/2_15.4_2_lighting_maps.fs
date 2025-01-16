
#version 330 core

out vec4 FragColor;

//propriet� del materiale dell'oggetto
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

//propriet� della luce
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // luce diffusa 
    //dipende dall'angolo tra la normale alla superficie (Normal) e la direzione della luce (lightDir) 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); //prodotto scalare tra la normale e la direzione della luce per determinare 
                                                //quanto la luce colpisce il punto
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    
    // luce speculare, simula i riflessi sulla superficie
    //Dipende dalla direzione della luce (lightDir), dalla normale e dalla posizione della camera (viewPos)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); //riflessione della direzione della luce rispetto alla normale
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //pow della lucentezza per simulare superfici + o - lucide
    vec3 specular = light.specular * spec * (vec3(1.0) - vec3(texture(material.specular, TexCoords))); //si stanno usando le
    //coordinate della texture. Si modifica la riflessione speculare in modo che dove la texture � chiara (specularit� alta) si ha
    //riflessione meno intensa
    
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
