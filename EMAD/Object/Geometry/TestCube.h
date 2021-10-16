#pragma once
#include "../EObject.h"

class TestCube : public EObject{
public:
	TestCube();

	virtual void draw(float* model, float* view, float* projection) override;
private:
	unsigned int texture1;
	unsigned int texture2;
};