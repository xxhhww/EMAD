#version 330 core
layout (location = 0) in vec3 posInWorld;

out vec4 vertexColor; //传入片段着色器

void main()
{
	gl_Position = vec4(posInWorld, 1.0f);
	vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);
}