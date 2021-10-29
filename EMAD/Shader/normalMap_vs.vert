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
    vec2 TexCoords;
    vec3 TangentFragPos;
    // view
    vec3 TangentViewPos;
    // π‚’’ƒ£–Õ
    vec3 TangentPointLightPos;
    vec3 TangentDirectLightDir;
} vs_out;

uniform mat4 model;

uniform vec3 PointLightPos;
uniform vec3 DirectLightDir;
uniform vec3 viewPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vs_out.FragPos = vec3(model * vec4(position, 1.0));   
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