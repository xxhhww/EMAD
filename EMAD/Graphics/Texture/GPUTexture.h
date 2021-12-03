#pragma once
#include "ShaderResource.h"

class GPUTexture : public ShaderResource {
public:
	using ptr = std::shared_ptr<GPUTexture>;
public:
	// 用作渲染目标的纹理的构造函数
	GPUTexture(const std::string& name, GPUSampler::ptr sampler, GPUTexDesc::ptr texDesc, GPUDevice* device);
};