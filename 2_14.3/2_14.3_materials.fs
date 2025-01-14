#version 330 core

out vec4 FragColor;

//proprietà del materiale dell'oggetto 
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

//proprietà della luce
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//input dal vs
in vec3 FragPos;  
in vec3 Normal;  

//varibili uniform
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // calcolo luce ambientale
    vec3 ambient = light.ambient * material.ambient;
  	
    // calcolo luce diffusa 
    //dipende dall'angolo tra la normale alla superficie (Normal) e la direzione della luce (lightDir)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); //prodotto scalare, misura quanto la normale è allineata con la luce
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // luce speculare, simula i riflessi lucidi sulla superficie
    //Dipende dalla direzione della luce (lightDir), dalla normale e dalla posizione della camera (viewPos)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);//riflessione della direzione della luce rispetto alla normale
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//pow della lucentezza per simulare superfici + o - lucide
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular; //componenti luci sommate insieme
    FragColor = vec4(result, 1.0); //colore finale + componente opaca
}
