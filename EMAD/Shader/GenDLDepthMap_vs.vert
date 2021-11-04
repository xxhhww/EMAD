#version 330 core
layout(location = 0) in vec3 vPos;

uniform mat4 model; //ģ�ͱ任����
uniform mat4 dlSpaceMat; //�����������µ�����任��������ͶӰ�ռ���

void main(){
	gl_Position = dlSpaceMat * model * vec4(vPos, 1.0f);
}