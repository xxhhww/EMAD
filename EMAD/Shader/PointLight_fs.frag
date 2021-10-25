#version 330 core
out vec4 FragColor;

uniform bool isSpec;

void main()
{
	if(isSpec){
		FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else{
		FragColor = vec4(0.5f, 0.5f, 0.3f, 1.0f);
	}
}