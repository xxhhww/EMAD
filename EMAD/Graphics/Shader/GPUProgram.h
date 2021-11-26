#pragma once
#include "../GPUResource.h"
#include "../Buffer/UniformBuffer.h"
#include "GPUShader.h"

#include <vector>

static const std::string sDirectory = "Shader/"; // ��ɫ������Ŀ¼

class GPUProgram : public GPUResource {
public:
	using ptr = std::shared_ptr<GPUProgram>;
public:
	GPUProgram(const std::string& name, GPUDevice* device)
		:GPUResource(name, ResourceType::Program, device){}

	// ���ļ��ж�ȡ��ɫ�����ݲ����룬�������mShaders������
	inline GPUShader::ptr AttachShader(ShaderType sType, const std::string& sName) {
		GPUShader::ptr temp = std::make_shared<GPUShader>(sType, sDirectory, sName, mDevice);
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
