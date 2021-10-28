#version 330 core
out vec4 FragColor;

in vec3 posInView;
in vec3 normalInView;
in vec2 texcoord;

void main()
{
	FragColor = vec4(0.5f, 0.3f, 0.7f, 1.0f);
}