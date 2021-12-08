#pragma once
#include "ShaderResource.h"

class GPUTexture : public ShaderResource {
public:
	using ptr = std::shared_ptr<GPUTexture>;
public:
	GPUTexture(const std::string& name, GPUDevice* device);
	// 创建渲染目标纹理
	void Update(GPUSampler::ptr sampler, GPUTexDesc::ptr texDesc);
};