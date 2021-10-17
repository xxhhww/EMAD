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

	std::pair<double, double> getCursorPos() const noexcept;
	void setCursorPos(double posX, double posY) noexcept;

	std::pair<double, double> getCursorOffset() const noexcept;
	void setCursorOffset(double offsetX, double offsetY) noexcept;
private:

private:
	int mWidth;
	int mHeight;
	std::string mName;
	GLFWwindow* mWindow; //����������

	// cursor last pos
	double mLastX;
	double mLastY;
	// cursor offset
	double mOffsetX;
	double mOffsetY;
};