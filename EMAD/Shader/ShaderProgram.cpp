#include "ShaderProgram.h"
#include "../Exception.h"

#include <glad/glad.h>

#include <sstream>

Program::Program(const std::string& vShader, const std::string& fShader)
{
	//从文件中读取着色器代码
	std::string vsCode;
	std::string fsCode;
	std::ifstream vsFile;
	std::ifstream fsFile;
	std::stringstream vsStream;
	std::stringstream fsStream;

	vsFile.open(vShader.c_str());
	fsFile.open(fShader.c_str());
	vsStream << vsFile.rdbuf();
	fsStream << fsFile.rdbuf();
	vsFile.close();
	fsFile.close();
	vsCode = vsStream.str();
	fsCode = fsStream.str();

	//编译着色器代码，链接着色器程序
	const char* vsCodeCstr = vsCode.c_str();
	const char* fsCodeCstr = fsCode.c_str();
	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[1024];
	//vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vsCodeCstr, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
		THROW_INFO_EXCEPTION("Failed to compile vertex shader\n");
	}
	//fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fsCodeCstr, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
		THROW_INFO_EXCEPTION("Failed to compile fragment shader\n");
	}
	//link shader program
	mProgram = glCreateProgram();
	glAttachShader(mProgram, vertexShader);
	glAttachShader(mProgram, fragmentShader);
	glLinkProgram(mProgram);
	glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mProgram, 1024, NULL, infoLog);
		THROW_INFO_EXCEPTION("Failed to link shader program");
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Program::~Program()
{
	glDeleteProgram(mProgram);
}

void Program::activate() const noexcept
{
	glUseProgram(mProgram);
}

void Program::setBool(const std::string& name, bool value) const noexcept
{
	glUniform1i(glGetUniformLocation(mProgram, name.c_str()), value);
}

void Program::setInt(const std::string& name, int value) const noexcept
{
	glUniform1i(glGetUniformLocation(mProgram, name.c_str()), value);
}

void Program::setFloat(const std::string& name, const float& value) const noexcept
{
	glUniform1f(glGetUniformLocation(mProgram, name.c_str()), value);
}

void Program::setVec3(const std::string& name, const glm::vec3& value) const noexcept
{
	glUniform3fv(glGetUniformLocation(mProgram, name.c_str()), 1, &value[0]);
}

void Program::setMatrix(const std::string& name, const glm::mat4& value) const noexcept
{
	glUniformMatrix4fv(glGetUniformLocation(mProgram, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
