#version 430

//calcola l'illuminazione di un oggetto nello spazio, tenendo conto dell'illuminazione ambientale, diffusa e speculare.

in vec3 vertPos;
in vec3 N;

uniform vec3 lightPos; // Posizione della luce
uniform vec3 lightIntensity; // Intensità della luce
uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;	// Ambient reflectivity 
uniform vec3 Ks;	// Specular reflectivity
uniform float specularShininess;	// Specular shininess 

float dotRV;

layout( location = 0 ) out vec4 FragColour;

vec3 ads()
{
	vec3 n = normalize(N);
		
	vec3 l = normalize( vec3(lightPos) - vertPos);

	float df = max( dot(l,n), 0.0); // Componente diffusa
	float sf = 0.0f;

	if (df > 0.0f) 
	{
		vec3 v = normalize(vec3(vertPos));
		vec3 r = reflect(l, n); // Calcola il riflesso
		sf = max(dot(r,v), 0.0f); // Componente speculare
	}

	// Ritorna il colore finale come combinazione dell'illuminazione ambientale, diffusa e speculare
	return lightIntensity * ( Ka + Kd * df  + Ks * pow( sf, specularShininess ));
}

void main() {

   FragColour = vec4(ads(),1.0); //colore finale frammento

}
