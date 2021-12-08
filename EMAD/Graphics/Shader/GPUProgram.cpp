#include "GPUProgram.h"

#include <iostream>
GPUProgram::GPUProgram(const std::string& name, GPUDevice* device)
	:GPUResource(name, ResourceType::Program, device){
	mResourceID = glCreateProgram();
}

GPUProgram::~GPUProgram(){
	glDeleteProgram(mResourceID);
}

void GPUProgram::Activate()
{
	if (!mIsLink) {
		for (const auto& i : mShaders) {
			glAttachShader(mResourceID, i->GetResourceID());
		}

		int success;
		char infoLog[1024];
		glLinkProgram(mResourceID);
		glGetProgramiv(mResourceID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(mResourceID, 1024, NULL, infoLog);
			std::cout << infoLog << std::endl;
			return;
		}
		mIsLink = true;
		mShaders.clear();
	}
	glUseProgram(mResourceID);
}
