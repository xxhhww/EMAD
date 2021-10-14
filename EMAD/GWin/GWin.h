#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

//������
class GWin {
public:
	GWin(int width, int height, const std::string& name);
	~GWin();

	GLFWwindow* window() noexcept { return mWindow; }
private:
	int mWidth;
	int mHeight;
	std::string mName;
	GLFWwindow* mWindow; //����������
};