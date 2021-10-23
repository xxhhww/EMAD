#pragma once

#include "../EMath.h"
#include <string>
#include <fstream>

//着色器程序类
class Program {
public:
	Program(const std::string& vShader, const std::string& fShader);
	~Program();

	void activate() const noexcept;

	void setInt(const std::string& name, int value) const noexcept;
	void setFloat(const std::string& name, const float& value) const noexcept;
	void setVec3(const std::string& name, const glm::vec3& value) const noexcept;
	void setMatrix(const std::string& name, float* value) const noexcept;
private:
	unsigned int mProgram;
};