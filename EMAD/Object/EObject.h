#pragma once
#include "../EMath.h"

#include <glad/glad.h>

#include <memory>

class Program;
class EObject {
public:
	virtual void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection) = 0;
	virtual ~EObject();

protected:
	unsigned int mVAO;
	unsigned int mIndexSize;
	std::shared_ptr<Program> mProgram;
};