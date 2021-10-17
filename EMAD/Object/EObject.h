#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

class Program;
class EObject {
public:
	virtual void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection) = 0;
	virtual ~EObject();

protected:
	unsigned int mVAO;
	std::shared_ptr<Program> mProgram;
};