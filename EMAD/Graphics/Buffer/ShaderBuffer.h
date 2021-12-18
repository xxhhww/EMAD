#pragma once

#include <Graphics/GPUResource.h>

// UniformBuffer”ÎStorageBufferµƒ∏∏¿‡
class ShaderBuffer : public GPUResource {
public:
	using ptr = std::shared_ptr<ShaderBuffer>;
public:
	ShaderBuffer(const std::string& name, ResourceType type, GPUDevice* device)
		:GPUResource(name, type, device){}

	~ShaderBuffer() {
		glDeleteBuffers(1, &mResourceID);
	}
};