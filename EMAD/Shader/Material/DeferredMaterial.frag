#version 450 core
// ��Ϊ���������µ���
layout(location = 0) out vec3 GBaseColor;
layout(location = 1) out vec3 GPosition;
layout(location = 2) out vec3 GNormal;
layout(location = 3) out vec3 GOther; // Metallic/Roughness/Ao

in FSIn{
	in vec3 fPosition;
	in vec2 fTexcoord;
	in vec3 fNormal;
} fsin;

// ��ȡ���û��ʲ�����
uniform sampler2D BaseColorTex;
uniform sampler2D MetallicTex;
uniform sampler2D RoughnessTex;
uniform sampler2D AoTex;

void main(){
	GBaseColor = texture(BaseColorTex, fsin.fTexcoord).rgb;
	GPosition = fsin.fPosition;
	GNormal = fsin.fNormal;
	
	GOther.r = texture(MetallicTex, fsin.fTexcoord).r;
	GOther.g = texture(MetallicTex, fsin.fTexcoord).g;
	GOther.b = texture(MetallicTex, fsin.fTexcoord).b;
}