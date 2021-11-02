#pragma once
#include "Drawable.hpp"

class TestPlane : public Drawable {
public:
	TestPlane(const std::string& name);

	virtual void draw(std::shared_ptr<Program> program) noexcept override;
private:
	unsigned int mDiffuseMap;
	unsigned int mNormalMap;
};