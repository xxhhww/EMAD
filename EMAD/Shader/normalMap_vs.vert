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

out VS_OUT {
    vec3 FragPos;
    vec3 plLightPos; //世界坐标下的点光源位置
    vec4 PosInDLSpace; //在定向光的投影空间中的顶点坐标
    vec2 TexCoords;
    vec3 TangentFragPos;
    // view
    vec3 TangentViewPos;
    // 光照模型
    vec3 TangentPointLightPos;
    vec3 TangentDirectLightDir;
} vs_out;

uniform mat4 model;
uniform mat4 dlSpaceMat;

uniform vec3 PointLightPos;
uniform vec3 DirectLightDir;
uniform vec3 viewPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vs_out.FragPos = vec3(model * vec4(position, 1.0f));
    vs_out.plLightPos = PointLightPos;
    vs_out.PosInDLSpace = dlSpaceMat * model * vec4(position, 1.0f);
    vs_out.TexCoords = texCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * normal);    
    
    mat3 TBN = transpose(mat3(T, B, N));  
    vs_out.TangentPointLightPos = TBN * PointLightPos;
    vs_out.TangentDirectLightDir = TBN * DirectLightDir;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
}