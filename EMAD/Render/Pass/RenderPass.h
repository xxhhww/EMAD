#pragma once

#include "../../Core/Singleton.h"
#include "../../Core/NonCopyable.h"

#include <memory>

class GPUProgram;

class RenderPassBase {
public:
	virtual void Init() = 0;
};

template<typename T>
class RenderPass : public Singleton<T>, public RenderPassBase, public NonCopyable {
};