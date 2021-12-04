#version 450 core
out vec4 FragColor;

in VSOut{
	vec3 fPosition;
	vec2 fTexcoord;
	vec3 fNormal;
} fsin;

uniform sampler2D DebugTex;

void main(){
	FragColor = vec4(texture(DebugTex, fsin.fTexcoord).rgb, 1.0f);
	//FragColor = vec4(0.8f, 0.3f, 0.5f, 1.0f);
}
