#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 posInView;
out vec3 normalInView;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	posInView = vec3(view * model * vec4(position, 1.0f));
	normalInView = vec3(view * model * vec4(normal, 0.0f));
}
