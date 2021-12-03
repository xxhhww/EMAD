#include "EApp.h"
#include "Object/Camera/Camera.h"

#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Shader/GPUProgram.h"

EApp::EApp(int width, int height, const std::string& name)
	:mWindow(width, height, name)
	,mDevicePtr(std::make_shared<GPUDevice>()){

	mCamera = std::make_shared<Camera>();
}

int EApp::Run()
{
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float lastFrame = 0.0f; // 上一帧的时间

	while (!glfwWindowShouldClose(mWindow.window()))
	{
		// HandleInput
		float aspect = (float)mWindow.getRectangle().first / (float)mWindow.getRectangle().second;
		mCamera->setAspect(aspect);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		this->HandleInput(deltaTime);

		// Clear Buffer & Fill Screen
		mDevicePtr->GetContext()->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		mDevicePtr->GetContext()->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Draw Objects
		VertexLayout tVertexLayout;
		tVertexLayout.Attach<AttrType::POSITION_3D>();
		tVertexLayout.Attach<AttrType::COLOR_RGB>();

		VertexDataArray tVertexDataArray{ tVertexLayout, 3u };
		tVertexDataArray[0].Attr<AttrType::POSITION_3D>() = { 0.0f, 0.5f, 0.0f };
		tVertexDataArray[1].Attr<AttrType::POSITION_3D>() = { 0.5f, 0.0f, 0.0f };
		tVertexDataArray[2].Attr<AttrType::POSITION_3D>() = { -0.5f, 0.0f, 0.0f };

		tVertexDataArray[0].Attr<AttrType::COLOR_RGB>() = { 1.0f, 0.0f, 0.0f };
		tVertexDataArray[1].Attr<AttrType::COLOR_RGB>() = { 0.0f, 1.0f, 0.0f };
		tVertexDataArray[2].Attr<AttrType::COLOR_RGB>() = { 0.0f, 0.0f, 1.0f };

		// Create VAO Without Index
		VertexBuffer::ptr tVertexBuffer = mDevicePtr->CreateVertexBuffer("testVAO", tVertexDataArray);
		// Create Shader Program
		GPUProgram::ptr tGPUProgram = mDevicePtr->CreateGPUProgram("testPrg");
		tGPUProgram->AttachShader(ShaderType::VS, "testTriangle_vs.vert");
		tGPUProgram->AttachShader(ShaderType::PS, "testTriangle_fs.frag");
		// Create Uniform Buffer
		struct TestStruct {
			glm::mat4 modelTrans;
			glm::mat4 viewTrans;
			glm::mat4 projTrans;
		};
		UniformBuffer::ptr tUniformBuffer = mDevicePtr->CreateUniformBuffer("testUB", GL_STATIC_DRAW, sizeof(TestStruct));
		TestStruct testStruct;
		testStruct.modelTrans = glm::identity<glm::mat4>();
		testStruct.viewTrans = mCamera->getView();
		testStruct.projTrans = mCamera->getProjection();
		tUniformBuffer->FillBuffer(0, sizeof(TestStruct), &testStruct);

		// Bind Resources For Rendering
		mDevicePtr->GetContext()->BindProgram(tGPUProgram);
		mDevicePtr->GetContext()->BindUB(0, tUniformBuffer);
		mDevicePtr->GetContext()->BindVAO(tVertexBuffer);

		mDevicePtr->GetContext()->DrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(mWindow.window());
		glfwPollEvents();
	}
	return 0;
}

void EApp::HandleInput(float dt)
{
	// esc为窗口退出键
	if (glfwGetKey(mWindow.window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(mWindow.window(), true);

	if (!mWindow.isCursorEnabled()) {
		// handle input
		if (glfwGetKey(mWindow.window(), GLFW_KEY_W) == GLFW_PRESS)
			mCamera->translate({ 0.0f, 0.0f, dt });
		if (glfwGetKey(mWindow.window(), GLFW_KEY_S) == GLFW_PRESS)
			mCamera->translate({ 0.0f, 0.0f, -dt });
		if (glfwGetKey(mWindow.window(), GLFW_KEY_A) == GLFW_PRESS)
			mCamera->translate({ -dt, 0.0f, 0.0f });
		if (glfwGetKey(mWindow.window(), GLFW_KEY_D) == GLFW_PRESS)
			mCamera->translate({ dt, 0.0f, 0.0f });

		const auto cursorOffset = mWindow.getCursorOffset();
		mWindow.setCursorOffset(0.0f, 0.0f);
		mCamera->rotate(cursorOffset.first, cursorOffset.second);
	}
}
