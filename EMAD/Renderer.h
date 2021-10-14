#pragma once
#include "GWin/GWin.h"

class App {
public:
	App(int width, int height, const std::string& name = "LearnOpenGL") noexcept;
	~App();

	int run();
private:
	GWin mWindow;
};