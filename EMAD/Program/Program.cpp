#include "Program.h"
#include "Shader.h"
#include "../Exception.h"

Program::Program(const std::string& vShader, const std::string& fShader, const std::string& gShader)
{
	unsigned int vertexShader = Shader::Load(GL_VERTEX_SHADER, vShader);
	unsigned int pixelShader = Shader::Load(GL_FRAGMENT_SHADER, fShader);
	unsigned int geometryShader = 0;
	if (!gShader.empty()) {
		geometryShader = Shader::Load(GL_GEOMETRY_SHADER, gShader);
	}

	mProgram = glCreateProgram();
	glAttachShader(mProgram, vertexShader);
	glAttachShader(mProgram, pixelShader);
	if (geometryShader != 0) {
		glAttachShader(mProgram, geometryShader);
	}

	int success;
	char infoLog[1024];
	glLinkProgram(mProgram);
	glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mProgram, 1024, NULL, infoLog);
		THROW_INFO_EXCEPTION("Failed to link shader program");
	}

	glDeleteShader(vertexShader);
	glDeleteShader(pixelShader);
	if (geometryShader != 0) {
		glDeleteShader(geometryShader);
	}
}

Program::~Program()
{
	glDeleteProgram(mProgram);
}

void Program::activate() const noexcept
{
	glUseProgram(mProgram);
}

const unsigned int Program::getProgram() const
{
	return mProgram;
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