#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

uniform vec3 inColour;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 vertexColour;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main(){
    vec4 worldPos = model * vec4(aPos, 1.0);
    gl_Position = projection * view * worldPos;

    vertexColour = inColour;
    TexCoord = aTexCoord;
    FragPos = vec3(worldPos);
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalize(normalMatrix * aNormal);
}