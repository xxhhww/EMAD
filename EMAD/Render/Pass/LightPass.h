#pragma once

#include "RenderPass.h"

class RenderContext;

class LightPass : public RenderPass {
public:
	// װ����ɫ������Դ
	virtual void Init() override;

	void Render(std::shared_ptr<RenderContext> rContext);
private:
};