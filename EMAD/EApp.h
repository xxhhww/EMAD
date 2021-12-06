#pragma once

#include <memory>
#include <string>

class Camera;
class EApp {
public:
	EApp(int width, int height, const std::string& name = "LearnOpenGL");

	int Run();

private:
	void DebugDeferredRun();
	void DebugSphereRun();
	void DebugQuadRun();
	void DebugTriangleRun();
	void DebugFrameBufferRun();

private:
	// ¥¶¿Ìº¸ Û ‰»Î
	void HandleInput(float dt);
private:
	std::shared_ptr<Camera> mCamera;
};