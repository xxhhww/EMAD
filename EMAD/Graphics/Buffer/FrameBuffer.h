#pragma once
#include "../GPUResource.h"
#include "../Texture/GPUTexture.h"

class FrameBuffer : public GPUResource {
public:
	FrameBuffer(const std::string& name, GPUDevice* device)
		:GPUResource(name, ResourceType::FrameBuffer, device) {
		glGenFramebuffers(1, &mResourceID);
	}

	// �����ɫ����
	inline void AttachColor(unsigned int index, std::shared_ptr<GPUTexture> tex) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, tex->GetResourceID(), 0);
	}

	// �����Ȼ���
	inline void AttachDepth(std::shared_ptr<GPUTexture> tex) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex->GetResourceID(), 0);
	}

	// ���ģ�建��
	inline void AttachStencil(std::shared_ptr<GPUTexture> tex) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, tex->GetResourceID(), 0);
	}

	// �����ȼ�ģ�建��
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