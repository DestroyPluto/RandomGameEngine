#version 330 core

in vec3 vertexColour;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;


void main()
{
//TODO: have these as uniforms instead of hardcoded
    float ambientStrength = 0.3;
    float diffuseStrength = 1.0;

    vec3 lightPos = vec3(0.0, -5.0, 0.0);
    vec3 lightColour = vec3(1.0, 1.0, 1.0);

    vec3 ambient = ambientStrength * lightColour;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColour;

    vec3 lightingColour = ambient + diffuse;
    
    vec3 finalColour = lightingColour * vertexColour;

    FragColor = vec4(finalColour, 1.0);
}