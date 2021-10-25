#pragma once

#include "Drawable.hpp"

class TestCube2 : public Drawable {
public:
	TestCube2();

	virtual void draw(std::shared_ptr<Program> program) noexcept override;
};