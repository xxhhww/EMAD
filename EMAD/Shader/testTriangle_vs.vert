#version 450 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexcoord;
layout (location = 2) in vec3 vColor;

layout (std140, binding = 0) uniform trans{
	mat4 model;
	mat4 view;
	mat4 projection;
};

out vec3 fColor;
out vec2 fTexcoord;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
	fColor = vColor;
	fTexcoord = vTexcoord;
}