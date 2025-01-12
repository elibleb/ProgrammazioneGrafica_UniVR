#version 330 core

out vec4 FragColor; // colore finale del frammento 

in vec3 Normal;  // normale del frammento, dal vs
in vec3 FragPos;     // posizione del frammento, dal vs

//variabili uniform
  
uniform vec3 lightPos; // posizione della luce
uniform vec3 viewPos; // posizione della telecamera
uniform vec3 lightColor; // colore della luce
uniform vec3 objectColor; // colore dell'oggetto

void main()
{
    // luce ambientale, luce che illumina uniformemente la scena
    float ambientStrength = 0.1; //intensità della luce ambientale, debole
    vec3 ambient = ambientStrength * lightColor; //colore: combinazione luce ambientale e colore luce
  	
    // luce diffusa, luce che colpisce direttamente la superficie
    vec3 norm = normalize(Normal); //normalizzazione, vettore unitario (normale lunghezza 1)
    vec3 lightDir = normalize(lightPos - FragPos); //calcolo direzione luce
    float diff = max(dot(norm, lightDir), 0.0); //prodotto scalare, quanto la luce colpisce la superficie
    vec3 diffuse = diff * lightColor; //calcolo colore diffuso
    
    // luce speculare, luce riflessa dalla superficie
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos); //direzione telecamera
    vec3 reflectDir = reflect(-lightDir, norm); //direzione riflessa dalla luce  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //intensità speculare
    vec3 specular = specularStrength * spec * lightColor; //colore speculare
        
    vec3 result = (ambient + diffuse + specular) * objectColor; //somma componenti di luce
    FragColor = vec4(result, 1.0); //colore finale con opacità
}