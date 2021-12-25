#pragma once
#include "GPUShader.h"

#include <Graphics/GPUResource.h>

#include <glm/glm.hpp>

#include <vector>
#include <map>

static const std::string sDirectory = "Shader/"; // ��ɫ������Ŀ¼

class GPUProgram : public GPUResource {
public:
	using ptr = std::shared_ptr<GPUProgram>;
public:
	GPUProgram(const std::string& name, GPUDevice* device);
	~GPUProgram();

	// ���ļ��ж�ȡ��ɫ�����ݲ����룬�������mShaders������
	inline GPUShader::ptr AttachShader(ShaderType sType, const std::string& sName) {
		// ��鵱ǰAttach��Shader�Ƿ��Ѿ���������
		for (const auto& i : mShaders) {
			if (i->GetShaderType() == sType) {
				return nullptr;
			}
		}

		GPUShader::ptr temp = std::make_shared<GPUShader>(sType, sDirectory, sName);
		mShaders.emplace_back(temp);
		return temp;
	}

	// ���ڴ��ַ����ж�ȡ��ɫ������
	inline GPUShader::ptr AttachShaderEx(ShaderType sType, const std::string& shaderCode) {
		// ��鵱ǰAttach��Shader�Ƿ��Ѿ���������
		for (const auto& i : mShaders) {
			if (i->GetShaderType() == sType) {
				return nullptr;
			}
		}

		GPUShader::ptr temp = std::make_shared<GPUShader>(sType, shaderCode);
		mShaders.emplace_back(temp);

		return temp;
	}


	// �������л�ö�Ӧ����ɫ������
	inline GPUShader::ptr GetShader(ShaderType sType) const {
		for (auto& i : mShaders) {
			if (i->GetShaderType() == sType)
				return i;
		}
	}

	// ��ʼ����ǰProgram
	inline void ReComplie() {
		glDeleteProgram(mResourceID);
		mIsLink = false;
		mResourceID = glCreateProgram();
	}

	// ����Uniform����...
	// PS������ǰ��Ҫ����Active()���������ǰ����
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
	bool mIsLink = false; // ��ɫ�������Ƿ��������
	std::vector<GPUShader::ptr> mShaders; // ��Ŷ�Ӧ����ɫ��

public:
	// [From GPUResource]
	virtual void Activate() override;
	// [From GPUResource]
	inline virtual void InActivate() override {
		glUseProgram(0);
	}
};
