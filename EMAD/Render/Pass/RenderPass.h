#pragma once

#include <memory>

class GPUProgram;

class RenderPass {
public:
	virtual void Init() = 0;
protected:
	std::shared_ptr<GPUProgram> mShaderProgram;
};