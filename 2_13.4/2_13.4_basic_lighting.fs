#version 330 core

out vec4 FragColor; // colore finale del frammento 

in vec3 Normal;      // normale del frammento, dal vs
in vec3 FragPos;     // posizione del frammento, dal vs


//variabili uniform

uniform vec3 lightPos; // posizione della luce
uniform vec3 lightColor; // colore della luce
uniform vec3 objectColor; // colore dell'oggetto


void main()
{
    // luce ambientale
    float ambientStrength = 0.1; //intensità della luce ambientale, debole
    vec3 ambient = ambientStrength * lightColor; //combinazione luce ambientale e colore luce
  	
    // luce diffusa 
    vec3 norm = normalize(Normal); //normalizzazione, vettore unitario (normale lunghezza 1)
    vec3 lightDir = normalize(lightPos - FragPos); //calcolo direzioene luce
    float diff = max(dot(norm, lightDir), 0.0); //prodotto scalare, determina quanto la luce colpisce la superficie
    vec3 diffuse = diff * lightColor;

    // combina l'illuminazione ambientale e diffusa con il colore dell'oggetto        
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
} 