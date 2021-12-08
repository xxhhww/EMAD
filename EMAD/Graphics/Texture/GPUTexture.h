#pragma once
#include "ShaderResource.h"

class GPUTexture : public ShaderResource {
public:
	using ptr = std::shared_ptr<GPUTexture>;
public:
	GPUTexture(const std::string& name, GPUDevice* device);
	// ������ȾĿ������
	void Update(GPUSampler::ptr sampler, GPUTexDesc::ptr texDesc);
};