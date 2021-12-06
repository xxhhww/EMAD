#pragma once
#include <Core/Singleton.h>
#include <Core/NonCopyable.h>

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//窗口类
class GWin : public Singleton<GWin>, public NonCopyable {
public:
	//GWin(int width, int height, const std::string& name);
	~GWin();
	void Init(int width, int height, const std::string& name);

	GLFWwindow* window() noexcept { return mWindow; }

	std::pair<int, int> getRectangle() const noexcept;
	void setRectangle(int width, int height) noexcept;

	std::pair<double, double> getCursorPos() const noexcept;
	void setCursorPos(double posX, double posY) noexcept;

	std::pair<double, double> getCursorOffset() const noexcept;
	void setCursorOffset(double offsetX, double offsetY) noexcept;

	bool isCursorEnabled() const noexcept { return mCursorEnabled; }
	void enableCursor() noexcept { mCursorEnabled = true; }
	void disableCursor() noexcept { mCursorEnabled = false; }

private:

private:
	int mWidth;
	int mHeight;
	std::string mName;
	GLFWwindow* mWindow; //窗口上下文

	// cursor last pos
	double mLastX;
	double mLastY;
	// cursor offset
	double mOffsetX;
	double mOffsetY;

	bool mCursorEnabled;
};