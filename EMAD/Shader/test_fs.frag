#version 330 core
out vec4 FragColor;

in vec2 Texcoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	FragColor = mix(texture(ourTexture1, Texcoord), texture(ourTexture2, Texcoord), 0.2);
}