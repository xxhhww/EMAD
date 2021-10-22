#pragma once

#include "../EObject.h"

class PointLight : public EObject {
public:
	PointLight();

	virtual void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;
private:
};