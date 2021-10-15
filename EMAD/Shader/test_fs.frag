#version 330 core
out vec4 outColor;

in vec4 vertexColor; //来自顶点着色器

void main()
{
	outColor = vertexColor;
}