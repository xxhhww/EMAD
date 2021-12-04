#pragma once

#include "RenderPass.h"

class RenderContext;

class LightPass : public RenderPass<LightPass> {
public:
	// 装载着色器等资源
	virtual void Init() override;

	void Render(std::shared_ptr<RenderContext> rContext);
private:
};