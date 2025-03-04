#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform vec3 inColour;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 vertexColour;
out vec2 TexCoord;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColour = inColour;
    TexCoord = aTexCoord;
}