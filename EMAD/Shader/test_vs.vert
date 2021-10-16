#version 330 core
layout (location = 0) in vec3 posInWorld;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texcoord;

out vec3 Color; //����Ƭ����ɫ��
out vec2 Texcoord;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(posInWorld, 1.0f);
	Color = color;
	Texcoord = texcoord;
}