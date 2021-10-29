#version 330 core
layout(location = 0) in vec3 vPos;

out vec3 Texcoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	Texcoord = vPos;
    gl_Position = projection * view * vec4(vPos, 1.0f);
}