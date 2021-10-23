#pragma once
#include "../EMath.h"
#include "../ImGui/imgui.h"

#include <glad/glad.h>

#include <memory>

class EObject {
public:
	virtual ~EObject() {}
	virtual void genCtrlGui() noexcept = 0;
};