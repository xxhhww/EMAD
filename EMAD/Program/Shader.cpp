#include "Shader.h"
#include "../Exception.h"

#include <fstream>
#include <sstream>

unsigned int Shader::Load(GLenum type, const std::string& fileName)
{
	//从文件中读取着色器代码
	std::string shaderCode;
	std::ifstream shaderFile;
	std::stringstream shaderStream;

	shaderFile.open(fileName.c_str());
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	shaderCode = shaderStream.str();

	//编译着色器代码，链接着色器程序
	const char* shaderCodeCstr = shaderCode.c_str();

	unsigned int shader;
	int success;
	char infoLog[1024];

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCodeCstr, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::string errorStr = "Failed to compile " + fileName + "\n";
		THROW_INFO_EXCEPTION(errorStr.c_str());
	}

	return shader;
}
