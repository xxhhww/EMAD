#version 450 core

out vec4 FragColor;

in VSOut{
	vec3 fPosition;
	vec2 fTexcoord;
} fsin;

layout (binding = 0) uniform sampler2D DebugTex;

void main(){
	FragColor = vec4(texture(DebugTex, fsin.fTexcoord).rgb, 1.0f);
	//FragColor = vec4(0.3f, 0.7f, 0.8f, 0.5f);
}