#pragma once
#include "ShaderResource.h"

class GPUTexture : public ShaderResource {
public:
	using ptr = std::shared_ptr<GPUTexture>;
public:
	// ������ȾĿ�������Ĺ��캯��
	GPUTexture(const std::string& name, GPUSampler::ptr sampler, GPUTexDesc::ptr texDesc, GPUDevice* device);
};