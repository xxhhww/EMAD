#pragma once
#include "../Drawable/Drawable.hpp"

class Program;
class PointLight : public Drawable{
public:
	PointLight();

	virtual void genCtrlGui() noexcept override;
	virtual void draw(std::shared_ptr<Program> program) noexcept override;

	const glm::vec3& getPosition() const { return mPosition; }
	const glm::vec3& getColor() const { return mColor; }
	const float& getAmbient() const { return mAmbient; }
	const float& getSpecular() const { return mSpecular; }

private:
	glm::vec3 mColor{ 1.0f, 1.0f, 1.0f };
	float mAmbient{ 0.3f };
	float mSpecular{ 0.5f };
};