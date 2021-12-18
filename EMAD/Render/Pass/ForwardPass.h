#pragma once

#include "RenderPass.h"

class RenderContext;

class ForwardPass : public RenderPass<ForwardPass> {
public:
	// 装配着色器等资源
	virtual void Init() override;
	// GBuffer渲染
	void Render(std::shared_ptr<RenderContext> rContext);
};