#pragma once

#include <string>
#include <fstream>

//着色器程序类
class Program {
public:
	Program(const std::string& vShader, const std::string& fShader);
	~Program();

	void activate();
private:
	unsigned int mProgram;
};