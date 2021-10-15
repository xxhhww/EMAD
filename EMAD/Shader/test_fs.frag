#version 330 core
out vec4 outColor;

in vec3 Color; //来自顶点着色器
in vec2 Texcoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	//outColor = texture(ourTexture1, Texcoord) * vec4(Color, 1.0f);
	outColor = mix(texture(ourTexture1, Texcoord), texture(ourTexture2, Texcoord), 0.2);
}