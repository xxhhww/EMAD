#pragma once
#include "GPUShader.h"

#include <Graphics/GPUResource.h>

#include <glm/glm.hpp>

#include <vector>
#include <map>

static const std::string sDirectory = "Shader/"; // 着色器所在目录

class GPUProgram : public GPUResource {
public:
	using ptr = std::shared_ptr<GPUProgram>;
public:
	GPUProgram(const std::string& name, GPUDevice* device);
	~GPUProgram();

	// 从文件中读取着色器数据并编译，最后存放在mShaders容器中
	inline GPUShader::ptr AttachShader(ShaderType sType, const std::string& sName) {
		// 检查当前Attach的Shader是否已经在容器中
		for (const auto& i : mShaders) {
			if (i->GetShaderType() == sType) {
				return nullptr;
			}
		}

		GPUShader::ptr temp = std::make_shared<GPUShader>(sType, sDirectory, sName);
		mShaders.emplace_back(temp);
		return temp;
	}

	// 从内存字符串中读取着色器数据
	inline GPUShader::ptr AttachShaderEx(ShaderType sType, const std::string& shaderCode) {
		// 检查当前Attach的Shader是否已经在容器中
		for (const auto& i : mShaders) {
			if (i->GetShaderType() == sType) {
				return nullptr;
			}
		}

		GPUShader::ptr temp = std::make_shared<GPUShader>(sType, shaderCode);
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

	// 初始化当前Program
	inline void ReComplie() {
		glDeleteProgram(mResourceID);
		mIsLink = false;
		mResourceID = glCreateProgram();
	}

	// 设置Uniform变量...
	// PS：设置前需要调用Active()函数，激活当前程序
	inline void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(mResourceID, name.c_str()), value);
	}

	inline void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(mResourceID, name.c_str()), value);
	}

	inline void setFloat(const std::string& name, const float& value) const {
		glUniform1f(glGetUniformLocation(mResourceID, name.c_str()), value);
	}

	inline void setVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(mResourceID, name.c_str()), 1, &value[0]);
	}

	inline void setMat4(const std::string& name, const glm::mat4& value) const {
		glUniformMatrix4fv(glGetUniformLocation(mResourceID, name.c_str()), 1, GL_FALSE, &value[0][0]);
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
