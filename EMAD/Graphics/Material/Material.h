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

	// 将材质的数据绑定到GPUContext
	void Bind(BindInfo& bindInfo);
private:
	MaterialParas mParas; // 着色器需要的材质参数
	std::shared_ptr<GPUProgram> mProgram; // 材质对应的着色器
	std::shared_ptr<UniformBuffer> mUniformBuffer0; // Shader Data
	std::shared_ptr<UniformBuffer> mUniformBuffer1; // Light Data (only for forward render)

	std::vector<char> mData0;
	std::vector<char> mData1;
};