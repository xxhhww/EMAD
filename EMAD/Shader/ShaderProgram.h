#pragma once

#include <string>
#include <fstream>

//��ɫ��������
class Program {
public:
	Program(const std::string& vShader, const std::string& fShader);
	~Program();

	void activate();
private:
	unsigned int mProgram;
};