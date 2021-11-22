#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexcoord;
layout (std140) uniform vpTrans{
	mat4 view;
	mat4 projection;
};

out vec3 posInView;
out vec3 posInWorld;
out vec4 posInDLSpace;
out vec3 normalInView;
out vec2 texcoord;

uniform mat4 model;
uniform mat4 dlSpaceMat;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
	posInView = vec3(view * model * vec4(vPosition, 1.0f));
	posInWorld vec3(model * vec4(vPosition, 1.0f));
	posInDLSpace = dlSpaceMat * model * vec4(vPosition, 1.0f);
	normalInView = vec3(view * model * vec4(vNormal, 0.0f));
	texcoord = vTexcoord;
}
