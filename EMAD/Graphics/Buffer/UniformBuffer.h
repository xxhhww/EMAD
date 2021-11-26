#pragma once
#include "../GPUResource.h"

class UniformBuffer : public GPUResource {
public:
	using ptr = std::shared_ptr<UniformBuffer>;

public:
	UniformBuffer(const std::string& name, GLenum usage, size_t bufferSize, GPUDevice* device, void* data = nullptr)
		:GPUResource(name, ResourceType::UniformBuffer, device) {

		glGenBuffers(1, &mResourceID);
		glBindBuffer(GL_UNIFORM_BUFFER, mResourceID);
		// 分配内存
		glBufferData(GL_UNIFORM_BUFFER, bufferSize, data, usage);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	// 填充数据
	inline void FillBuffer(size_t offset, size_t size, void* data = nullptr) {
		Activate();
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

public:
	// [From GPUResource]
	inline virtual void Activate() override {
		glBindBuffer(GL_UNIFORM_BUFFER, mResourceID);
	}
	// [From GPUResource]
	inline virtual void InActivate() override {
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
};