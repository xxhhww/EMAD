#version 330 core
layout(location = 0) in vec3 vPos;
layout (std140) uniform vpTrans{
	mat4 view;
	mat4 projection;
};

uniform mat4 realView;
out vec3 Texcoord;

void main()
{
	Texcoord = vPos;
    gl_Position = projection * realView * vec4(vPos, 1.0f);
}