#pragma once

#include "Drawable.hpp"

class TestCube2 : public Drawable {
public:
	TestCube2();

	virtual void draw(glm::mat4 view, glm::mat4 projection) override;
};