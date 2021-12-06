#version 450 core
// 均为世界坐标下的量
layout(location = 0) out vec3 GBaseColor;
layout(location = 1) out vec3 GPosition;
layout(location = 2) out vec3 GNormal;
layout(location = 3) out vec3 GOther; // Metallic/Roughness/Ao

in VSOut{
	in vec3 fPosition;
	in vec2 fTexcoord;
	in vec3 fNormal;
} fsin;

// 读取自用户资产纹理
layout(binding = 0) uniform sampler2D BaseColorTex;
layout(binding = 1) uniform sampler2D NormalTex;
layout(binding = 2) uniform sampler2D MetallicTex;
layout(binding = 3) uniform sampler2D RoughnessTex;
layout(binding = 4) uniform sampler2D AoTex;

void main(){
	GBaseColor = texture(BaseColorTex, fsin.fTexcoord).rgb;
	// GBaseColor = vec3(0.3f, 0.4f, 0.5f);
	GPosition = fsin.fPosition;
	GNormal = fsin.fNormal;
	
	GOther.r = texture(MetallicTex, fsin.fTexcoord).r;
	GOther.g = texture(MetallicTex, fsin.fTexcoord).g;
	GOther.b = texture(MetallicTex, fsin.fTexcoord).b;
}