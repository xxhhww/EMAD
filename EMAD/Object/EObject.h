#pragma once

#include <glad/glad.h>

#include <memory>

class Program;
class EObject {
public:
	virtual void draw(float* model, float* view, float* projection) = 0;
	virtual ~EObject();

protected:
	unsigned int mVAO;
	std::shared_ptr<Program> mProgram;
};