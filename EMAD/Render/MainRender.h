#pragma once

#include <Core/NonCopyable.h>
#include <Core/Singleton.h>

#include <memory>
#include <map>

class RenderContext;

// Ö÷äÖÈ¾Æ÷
class RenderService : public Singleton<RenderService>, public NonCopyable {
public:
	using ptr = std::shared_ptr<RenderService>;
public:
	// All RenderPass Init
	void Init();
	void Render(std::shared_ptr<RenderContext> rContext);
private:
};