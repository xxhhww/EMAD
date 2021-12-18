// 点光源的固体球
#version 450 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexcoord;
layout(location = 2) in vec3 vNormal;

layout(std140, binding = 0) uniform Trans{
	mat4 modelTrans;
	mat4 viewTrans;
	mat4 projTrans;
	vec4 lightColor;
};

out VSOut{
	vec3 fPosition;
	vec2 fTexcoord;
	vec3 fNormal;
	vec4 fLightColor;
} vsout;

void main(){
	gl_Position = projTrans * viewTrans * modelTrans * vec4(vPosition, 1.0f);

	vsout.fPosition = vec3(modelTrans * vec4(vPosition, 1.0f));
	vsout.fTexcoord = vTexcoord;
	vsout.fNormal = mat3(modelTrans) * vec3(vNormal);

	vsout.fLightColor = lightColor;
}