#pragma once

#include "../EObject.h"

class DirectLight : public EObject {
public:
	virtual void genCtrlGui() noexcept override;

	const glm::vec3& getDirection() const { return mDirection; }
	const glm::vec3& getColor() const { return mColor; }
	const float& getAmbient() const { return mAmbient; }
	const float& getSpecular() const { return mSpecular; }
private:
	glm::vec3 mDirection{ 2.0f, -2.0f, -2.0f };
	glm::vec3 mColor{ 1.0f, 1.0f, 1.0f };
	float mAmbient{ 0.3f };
	float mSpecular{ 0.5f };
};