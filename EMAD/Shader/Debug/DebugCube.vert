#version 450 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexcoord;

layout(std140, binding = 0) uniform DebugTrans{
	mat4 modelTrans;
	mat4 viewTrans;
	mat4 projTrans;
};

out VSOut{
	vec3 fPosition;
	vec2 fTexcoord;
} vsout;

void main(){
	gl_Position = projTrans * viewTrans * modelTrans * vec4(vPosition, 1.0f);
	vsout.fPosition = vPosition;
	vsout.fTexcoord = vTexcoord;
}