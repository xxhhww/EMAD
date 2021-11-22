#pragma once
#include "../GPUResource.h"

class UniformBuffer : public GPUResource {
public:
	UniformBuffer(const std::string& name, GLenum usage, size_t bufferSize, GPUDevice* device, void* data = nullptr)
		:GPUResource(name, ResourceType::UniformBuffer, device) {

		glGenBuffers(1, &mResourceID);
		glBindBuffer(GL_UNIFORM_BUFFER, mResourceID);
		// �����ڴ�
		glBufferData(GL_UNIFORM_BUFFER, bufferSize, data, usage);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	// �������
	inline void FillBuffer(size_t offset, size_t size, void* data = nullptr) {
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

	// ��UBO����󶨵���Ӧ��Uniform����
	inline void BindBuffer(unsigned int bindIndex, size_t offset, size_t size) {
		glBindBufferRange(GL_UNIFORM_BUFFER, bindIndex, mResourceID, offset, size);
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