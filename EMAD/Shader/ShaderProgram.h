#pragma once

#include <string>
#include <fstream>

//��ɫ��������
class Program {
public:
	Program(const std::string& vShader, const std::string& fShader);
	~Program();

	void activate() const noexcept;

	void setInt(const std::string& name, int value) const noexcept;
	void setMatrix(const std::string& name, float* value) const noexcept;
private:
	unsigned int mProgram;
};