#pragma once
#include "ShaderBuffer.h"

// SSBO
class StorageBuffer : public ShaderBuffer {
public:
	using ptr = std::shared_ptr<StorageBuffer>;

public:
	StorageBuffer(const std::string& name, GPUDevice* device)
		:ShaderBuffer(name, ResourceType::StorageBuffer, device) {
		glGenBuffers(1, &mResourceID);
	}

	// ∑÷≈‰/–ﬁ∏ƒ¥Ê¥¢ø’º‰
	inline void AllocBuffer(size_t spaceSize, GLenum usage, void* data = nullptr) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mResourceID);
		// ∑÷≈‰ƒ⁄¥Ê
		glBufferData(GL_SHADER_STORAGE_BUFFER, spaceSize, data, usage);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	// ÃÓ≥‰¥Ê¥¢ø’º‰
	inline void FillBuffer(size_t offset, size_t size, void* data = nullptr) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mResourceID);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
		// glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

public:
	// [From GPUResource]
	inline virtual void Activate() override {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mResourceID);
	}
	// [From GPUResource]
	inline virtual void InActivate() override {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
};