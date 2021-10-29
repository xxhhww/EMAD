#version 330 core
out vec4 FragColor;

in vec3 Texcoord;

uniform samplerCube cubeMap;

void main()
{    
    FragColor = texture(cubeMap, Texcoord);
}