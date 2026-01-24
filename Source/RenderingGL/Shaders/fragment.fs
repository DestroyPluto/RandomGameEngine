#version 330 core

in vec3 vertexColour;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex;

void main()
{
	//FragColor = vec4(vertexColour,1.0);
	FragColor = texture(tex, TexCoord) * vec4(vertexColour,1.0);
}