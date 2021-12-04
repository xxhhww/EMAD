#pragma once
#include "Graphics/GPUDevice.h"
#include "Graphics/GPUContext.h"
#include "GWin/GWin.h"

class Camera;
class EApp {
public:
	EApp(int width, int height, const std::string& name = "LearnOpenGL");

	int Run();

private:
	void DebugPBRRun();
	void DebugSphereRun();
	void DebugTriangleRun();

private:
	// ¥¶¿Ìº¸ Û ‰»Î
	void HandleInput(float dt);
private:
	GWin mWindow;

	std::shared_ptr<Camera> mCamera;
};