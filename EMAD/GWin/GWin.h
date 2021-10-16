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

	std::pair<int, int> getRectangle() const noexcept;
	void setRectangle(int width, int height) noexcept;
private:
	int mWidth;
	int mHeight;
	std::string mName;
	GLFWwindow* mWindow; //����������
};