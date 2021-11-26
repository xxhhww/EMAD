#pragma once
#include "../GPUResource.h"
#include "../Buffer/UniformBuffer.h"
#include "GPUShader.h"

#include <vector>

static const std::string sDirectory = "Shader/"; // 着色器所在目录

class GPUProgram : public GPUResource {
public:
	using ptr = std::shared_ptr<GPUProgram>;
public:
	GPUProgram(const std::string& name, GPUDevice* device)
		:GPUResource(name, ResourceType::Program, device){}

	// 从文件中读取着色器数据并编译，最后存放在mShaders容器中
	inline GPUShader::ptr AttachShader(ShaderType sType, const std::string& sName) {
		GPUShader::ptr temp = std::make_shared<GPUShader>(sType, sDirectory, sName, mDevice);
		mShaders.emplace_back(temp);
		return temp;
	}
	// 从容器中获得对应的着色器对象
	inline GPUShader::ptr GetShader(ShaderType sType) const {
		for (auto& i : mShaders) {
			if (i->GetShaderType() == sType)
				return i;
		}
	}

private:
	bool mIsLink = false; // 着色器程序是否链接完毕
	std::vector<GPUShader::ptr> mShaders; // 存放对应的着色器

public:
	// [From GPUResource]
	virtual void Activate() override;
	// [From GPUResource]
	inline virtual void InActivate() override {
		glUseProgram(0);
	}
};
