#pragma once

#include "RenderPass.h"

class RenderContext;

class GBufferPass : public RenderPass<GBufferPass> {
public:
	// 装配着色器等资源
	virtual void Init() override;
	// GBuffer渲染
	void Render(std::shared_ptr<RenderContext> rContext);
};