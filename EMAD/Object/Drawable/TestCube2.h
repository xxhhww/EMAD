#pragma once

#include "Drawable.hpp"

class TestCube2 : public Drawable {
public:
	TestCube2(const std::string& name);

	virtual void draw(std::shared_ptr<Program> program) noexcept override;
private:
	unsigned int texture1;
	unsigned int texture2;
};