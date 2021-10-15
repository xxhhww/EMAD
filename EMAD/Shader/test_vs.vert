#version 330 core
layout (location = 0) in vec3 posInWorld;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texcoord;

out vec3 Color; //传入片段着色器
out vec2 Texcoord;

void main()
{
	gl_Position = vec4(posInWorld, 1.0f);
	Color = color;
	Texcoord = texcoord;
}