#pragma once

#include "RenderPass.h"

class GPUContext;
class RenderContext;
class FrameBuffer;

class GBufferPass : public RenderPass<GBufferPass> {
public:
	// װ����ɫ������Դ
	virtual void Init() override;
	// GBuffer��Ⱦ
	void Render(std::shared_ptr<RenderContext> rContext);
private:
	std::shared_ptr<FrameBuffer> mFrameBuffer;
};