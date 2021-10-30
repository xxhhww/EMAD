#pragma once
#include <glad/glad.h>

#include <string>

class Shader {
public:
	static unsigned int Load(GLenum type, const std::string& fileName);
protected:
};