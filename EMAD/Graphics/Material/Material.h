#pragma once

#include "../GPUContext.h"
#include "../Shader/GPUProgram.h"
#include "../Buffer/UniformBuffer.h"
#include "MaterialParameter.h"

class Material {
public:
	struct BindInfo {
		std::shared_ptr<GPUContext> Context;
	};

public:
	Material();

	// �����ʵ����ݰ󶨵�GPUContext
	void Bind(BindInfo& bindInfo);
private:
	MaterialParas mParas; // ��ɫ����Ҫ�Ĳ��ʲ���
	std::shared_ptr<GPUProgram> mProgram; // ���ʶ�Ӧ����ɫ��
	std::shared_ptr<UniformBuffer> mUniformBuffer0; // Shader Data
	std::shared_ptr<UniformBuffer> mUniformBuffer1; // Light Data (only for forward render)

	std::vector<char> mData0;
	std::vector<char> mData1;
};