#version 450 core
layout(location = 0) out vec3 GBaseColor;

in VSOut{
	in vec3 fPosition;
	in vec2 fTexcoord;
	in vec3 fNormal;
} fsin;

void main(){
	GBaseColor = fsin.fPosition;
}