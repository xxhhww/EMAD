#version 450 core

out vec4 FragColor;

in vec3 fColor;
in vec2 fTexcoord;

layout (binding = 0) uniform sampler2D DebugTex;

void main()
{
	FragColor = texture2D(DebugTex, fTexcoord);
}
