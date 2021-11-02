#pragma once
#include "GWin/GWin.h"
#include "Object/Camera/Camera.h"

#include <memory>

class SkyBox;
class TestCube2;
class TestPlane;
class PointLight;
class DirectLight;

class App {
public:
	App(int width, int height, const std::string& name = "LearnOpenGL") noexcept;
	~App();

	int run();
private:
	void handleInput(float dt) noexcept;
	void genCtrlGui() const noexcept;
private:
	GWin mWindow;
	std::shared_ptr<SkyBox> mSkyBox;
	std::shared_ptr<Camera> mCamera;
	std::shared_ptr<PointLight> mPointLight;
	std::shared_ptr<DirectLight> mDirectLight;

	std::shared_ptr<TestCube2> mTestCube1;
	std::shared_ptr<TestCube2> mTestCube2;
	std::shared_ptr<TestCube2> mTestCube3;
	std::shared_ptr<TestPlane> mTestPlane1;
};