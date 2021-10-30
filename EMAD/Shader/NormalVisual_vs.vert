#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

layout (std140) uniform vpTrans{
	mat4 view;
	mat4 projection;
};
uniform mat4 model;

out VS_OUT{
	vec3 normal;
} vs_out;

void main(){
	gl_Position = projection * view * model * vec4(position ,1.0f);
	// 几何着色器在裁剪空间中进行计算，因此，法向量必须转换到裁剪空间中
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(projection * vec4(normalMatrix * normal, 0.0f)));
}