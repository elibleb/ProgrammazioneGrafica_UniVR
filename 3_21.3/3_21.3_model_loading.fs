#version 330 core

out vec4 FragColor;

in vec2 TexCoords; //coordinate texture passate dal vs

uniform sampler2D texture_diffuse1; //texture da utilizzare, uniform

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords); // campiona la texture e la assegna a FragColor
}