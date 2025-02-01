#version 430

layout (location = 0) in vec3 VertexPosition; // stream delle posizioni dei vertici  
layout (location = 1) in vec3 VertexNormal; // stream delle normali dei vertici

layout (location=2) in ivec4 BoneIDs; // Stream vertex bone IDs
layout (location=3) in vec4 Weights; // Stream pesi vertici

out vec3 vertPos; // Posizione del vertice nelle coordinate della fotocamera
out vec3 N; // Normale trasformata

uniform mat3 NormalMatrix; // Normal matrix 
uniform mat4 M; // Model matrix 
uniform mat4 V; // View matrix 
uniform mat4 P; // Projection matrix 

const int MAX_BONES = 70; //  Numero massimo di ossa (bones)
uniform mat4 gBones[MAX_BONES]; // Trasformazioni delle ossa

void main()
{
	//  Moltiplica ogni trasformazione dell'osso per il peso
	// e combinali. 
   	mat4 BoneTransform = gBones[ BoneIDs[0] ] * Weights[0];
	BoneTransform += gBones[ BoneIDs[1] ] * Weights[1];
    BoneTransform += gBones[ BoneIDs[2] ] * Weights[2];
    BoneTransform += gBones[ BoneIDs[3] ] * Weights[3];

	// Posizione del vertice trasformatan 
	vec4 tPos = BoneTransform * vec4(VertexPosition, 1.0);

	gl_Position = (P * V * M) * tPos;

	// Normale trasformata 
	vec4 tNormal = BoneTransform * vec4(VertexNormal, 0.0);

	N = normalize( mat4(NormalMatrix) * tNormal).xyz;

    vec4 worldPos = M * tPos;

	vertPos = worldPos.xyz;     
}