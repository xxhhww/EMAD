#pragma once

#include "../Core/NonCopyable.h"

#include <memory>
#include <map>

class RenderContext;

// ����Ⱦ��
class MainRenderer : public NonCopyable {
public:
	using ptr = std::shared_ptr<MainRenderer>;
public:
	// All RenderPass Init
	void Init();
	void Render(std::shared_ptr<RenderContext> rContext);
private:
};