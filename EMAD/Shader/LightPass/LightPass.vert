#version 450 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexcoord;

out VSOut{
	vec3 fPosition;
	vec2 fTexcoord;
} vsout;

void main(){
	gl_Position = vec4(vPosition, 1.0f);
	vsout.fPosition = vPosition;
	vsout.fTexcoord = vTexcoord;
}