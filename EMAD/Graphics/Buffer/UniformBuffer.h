#pragma once
#include <Graphics/GPUResource.h>

class UniformBuffer : public GPUResource {
public:
	using ptr = std::shared_ptr<UniformBuffer>;

public:
	UniformBuffer(const std::string& name, GPUDevice* device)
		:GPUResource(name, ResourceType::UniformBuffer, device) {
		glGenBuffers(1, &mResourceID);
	}

	// ∑÷≈‰/–ﬁ∏ƒ¥Ê¥¢ø’º‰
	inline void AllocBuffer(size_t spaceSize, GLenum usage, void* data = nullptr) {
		glBindBuffer(GL_UNIFORM_BUFFER, mResourceID);
		// ∑÷≈‰ƒ⁄¥Ê
		glBufferData(GL_UNIFORM_BUFFER, spaceSize, data, usage);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	// ÃÓ≥‰¥Ê¥¢ø’º‰
	inline void FillBuffer(size_t offset, size_t size, void* data = nullptr) {
		glBindBuffer(GL_UNIFORM_BUFFER, mResourceID);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		
		// glBindBuffer(GL_UNIFORM_BUFFER, 0);
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