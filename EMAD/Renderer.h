#pragma once
#include "GWin/GWin.h"
#include "Object/Camera/Camera.h"

#include <memory>

class App {
public:
	App(int width, int height, const std::string& name = "LearnOpenGL") noexcept;
	~App();

	int run();
private:
	void handleInput(float dt) noexcept;
private:
	GWin mWindow;
	std::shared_ptr<Camera> mCamera;
};