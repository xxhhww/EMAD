#pragma once
#include "../EObject.h"

class TestCube : public EObject{
public:
	TestCube();

	virtual void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;
private:
	unsigned int texture1;
	unsigned int texture2;
};