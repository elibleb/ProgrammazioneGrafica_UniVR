#version 330 core

out vec4 FragColor; // colore finale del frammento

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

//variabili uniform

uniform vec3 lightColor; // colore della luce
uniform vec3 objectColor; // colore dell'oggetto

void main()
{
    // luce ambientale, luce che illumina uniformemente la scena
    float ambientStrength = 0.1; //intensità
    vec3 ambient = ambientStrength * lightColor; //colore: combinazione luce ambientale e colore luce
  	
    // luce diffusa, luce che colpisce direttamente la superfici
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos); // direzione della luce dallo spazio vista, LightPos e FragPos sono 
                                                   //nello spazio view
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor; //calcolo colore diffuso
    
    // luce speculare, luce riflessa dalla superficie
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos); // Direzione view nello spazio della vista, i frammenti sono già nello spazion vista
    vec3 reflectDir = reflect(-lightDir, norm); // direzione riflessa della luce
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //intensità speculare
    vec3 specular = specularStrength * spec * lightColor; //colore speculare 
        
    vec3 result = (ambient + diffuse + specular) * objectColor; // somma dell'illuminazione
    FragColor = vec4(result, 1.0); //colore finale con opacità
}