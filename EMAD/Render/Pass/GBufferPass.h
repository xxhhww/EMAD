#pragma once

#include "RenderPass.h"

class RenderContext;

class GBufferPass : public RenderPass<GBufferPass> {
public:
	// װ����ɫ������Դ
	virtual void Init() override;
	// GBuffer��Ⱦ
	void Render(std::shared_ptr<RenderContext> rContext);
};