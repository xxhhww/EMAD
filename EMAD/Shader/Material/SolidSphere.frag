#version 450 core

layout(location = 0) out vec4 FragColor;

in VSOut{
	vec3 fPosition;
	vec2 fTexcoord;
	vec3 fNormal;
	vec4 fLightColor;
} fsin;

void main(){
	FragColor = fsin.fLightColor;
}

