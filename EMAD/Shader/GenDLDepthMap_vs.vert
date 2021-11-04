#version 330 core
layout(location = 0) in vec3 vPos;

uniform mat4 model; //模型变换矩阵
uniform mat4 dlSpaceMat; //将世界坐标下的物体变换到定向光的投影空间中

void main(){
	gl_Position = dlSpaceMat * model * vec4(vPos, 1.0f);
}