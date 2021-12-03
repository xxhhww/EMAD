#version 450 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vTexcoord;
layout(location = 2) in vec3 vNormal;

layout(std140, binding = 0) uniform Trans{
	mat4 modelTrans;
	mat4 viewTrans;
	mat4 projTrans;
};

out VSOut{
	vec3 fPosition;
	vec3 fTexcoord;
	vec3 fNormal;
} vsout;

void main(){
	gl_Position = projTrans * viewTrans * modelTrans * vec4(vPosition, 1.0f);

	vsout.fPosition = vec3(modelTrans * vec4(vPosition, 1.0f));
	vsout.fTexcoord = vTexcoord;
	vsout.fNormal = vNormal;
}