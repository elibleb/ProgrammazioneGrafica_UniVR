#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; //normale del vertice in input

//output

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;

//varibili uniform

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    FragPos = vec3(view * model * vec4(aPos, 1.0));// La posizione del frammento nello spazio della vista

    Normal = mat3(transpose(inverse(view * model))) * aNormal; // Trasforma la normale nello spazio della vista

    LightPos = vec3(view * vec4(lightPos, 1.0)); // Trasforma la posizione world-space della luce in posizione view-space,
    //la luce e la fotocamera vengono trattate come se fossero nel sistema di coordinate della fotocamera (view space)
}