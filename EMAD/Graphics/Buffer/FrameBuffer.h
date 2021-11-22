#pragma once
#include "../GPUResource.h"
#include "../Texture/GPUTexture.h"

class FrameBuffer : public GPUResource {
public:
	FrameBuffer(const std::string& name, GPUDevice* device)
		:GPUResource(name, ResourceType::FrameBuffer, device) {
		glGenFramebuffers(1, &mResourceID);
	}

	// 添加颜色附件
	inline void AttachColor(unsigned int index, std::shared_ptr<GPUTexture> tex) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, tex->GetResourceID(), 0);
	}

	// 添加深度缓存
	inline void AttachDepth(std::shared_ptr<GPUTexture> tex) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex->GetResourceID(), 0);
	}

	// 添加模板缓存
	inline void AttachStencil(std::shared_ptr<GPUTexture> tex) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, tex->GetResourceID(), 0);
	}

	// 添加深度及模板缓存
	// 24-8
	inline void AttachDepth24Stencil8(std::shared_ptr<GPUTexture> tex) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, tex->GetResourceID(), 0);
	}
	// 32-8
	inline void AttachDepth32Stencil8(std::shared_ptr<GPUTexture> tex) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH32F_STENCIL8, tex->GetResourceID(), 0);
	}

public:
	// [From GPUResource]
	inline virtual void Activate() override {
		glBindBuffer(GL_FRAMEBUFFER, mResourceID);
	}
	// [From GPUResource]
	inline virtual void InActivate() override {
		glBindBuffer(GL_FRAMEBUFFER, 0);
	}
};