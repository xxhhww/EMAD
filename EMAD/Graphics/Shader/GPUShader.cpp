#include "GPUShader.h"

#include <fstream>
#include <sstream>
#include <iostream>

GPUShader::GPUShader(ShaderType sType, const std::string& dir, const std::string& sName, GPUDevice* device)
	:GPUResource(sName, ResourceType::Shader, device) {
		mShaderType = sType;

		//从文件中读取着色器代码
		std::string shaderCode;
		const char* shaderCodeCStyle;

		std::ifstream shaderFile;
		std::stringstream shaderStream;

		shaderFile.open(dir + sName);
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
		shaderCodeCStyle = shaderCode.c_str();

		switch (sType)
		{
		case ShaderType::VS:
			mResourceID = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::GS:
			mResourceID = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case ShaderType::PS:
			mResourceID = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case ShaderType::Other:
		default:
			std::cout << "ShaderType Is Not Exist" << std::endl;
			break;
		}

		int success;
		char infoLog[1024];
		glShaderSource(mResourceID, 1, &shaderCodeCStyle, nullptr);
		glCompileShader(mResourceID);
		glGetShaderiv(mResourceID, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(mResourceID, 1024, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}
	}
