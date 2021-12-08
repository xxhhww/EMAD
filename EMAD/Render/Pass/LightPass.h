#pragma once

#include "RenderPass.h"

class GPUProgram;
class RenderContext;

class LightPass : public RenderPass<LightPass> {
public:
	~LightPass();

	// װ����ɫ������Դ
	virtual void Init() override;

	void Render(std::shared_ptr<RenderContext> rContext);
private:
	std::shared_ptr<GPUProgram> mShaderProgram;
};