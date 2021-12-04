#include "EApp.h"
#include "Object/Camera/Camera.h"
#include <Graphics/Texture/AssetTexture.h>
#include <Graphics/Buffer/VertexBuffer.h>
#include <Graphics/Shader/GPUProgram.h>
#include <Geometry/Sphere.h>
#include <Geometry/Cube.h>
#include <Render/MainRender.h>
#include <Render/RenderContext.h>

EApp::EApp(int width, int height, const std::string& name)
	:mWindow(width, height, name){
	mCamera = std::make_shared<Camera>();
	// 启动深度缓冲测试
	glEnable(GL_DEPTH_TEST);
	// 启动模板测试
	glEnable(GL_STENCIL_TEST);
	// 初始化渲染服务
	RenderService::Instance()->Init();
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
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		this->HandleInput(deltaTime);
		
		//DebugTriangleRun();
		DebugSphereRun();

		glfwSwapBuffers(mWindow.window());
		glfwPollEvents();
	}
	return 0;
}

// Debug Function

struct DebugTrans {
	glm::mat4 modelTrans;
	glm::mat4 viewTrans;
	glm::mat4 projTrans;
};

void EApp::DebugPBRRun()
{
	//RenderContext::ptr CurrentRenderContext 
}

void EApp::DebugSphereRun()
{
	// Clear Buffer & Fill Screen
	GPUDevice::Instance()->GetContext()->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GPUDevice::Instance()->GetContext()->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Draw Sphere
	
	// Get VB
	VertexBuffer::ptr SphereVertexBuffer = Sphere::GetVB();
	unsigned int indexSize = SphereVertexBuffer->GetIndexSize();

	// Create ShaderProgram
	GPUProgram::ptr SphereProgram = GPUDevice::Instance()->CreateGPUProgram("Program_DebugSphere");
	SphereProgram->AttachShader(ShaderType::VS, "Debug/DebugSphere.vert");
	SphereProgram->AttachShader(ShaderType::PS, "Debug/DebugSphere.frag");

	// 设置对应的纹理单元
	glUniform1i(glGetUniformLocation(SphereProgram->GetResourceID(), "DebugTex"), 0);

	// Create UniformBuffer
	UniformBuffer::ptr SphereUniform = GPUDevice::Instance()->CreateUniformBuffer("UB_DebugSphere", GL_STATIC_DRAW, sizeof(DebugTrans));
	DebugTrans debugTrans;

	glm::mat4 modelTrans{ 1.0f };
	glm::mat4 identity{ 1.0f };
	modelTrans *= glm::translate(identity, glm::vec3{0.0f, 0.0f, 0.0f});
	modelTrans *= glm::rotate(identity, glm::radians(-90.0f), glm::vec3{ 1.0f, 0.0f, 0.0f });
	modelTrans *= glm::rotate(identity, 00.0f, glm::vec3{ 0.0f, 1.0f, 0.0f });
	modelTrans *= glm::rotate(identity, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });

	debugTrans.modelTrans = modelTrans;
	debugTrans.viewTrans = mCamera->getView();
	debugTrans.projTrans = mCamera->getProjection();
	SphereUniform->FillBuffer(0, sizeof(DebugTrans), &debugTrans);

	// Create AssetTexture
	GPUSampler::ptr SphereTexSampler = GPUSampler::Gen2D(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	AssetTexture::ptr SphereAssetTex = GPUDevice::Instance()->CreateAssetTexture2D("earth2048.bmp", SphereTexSampler);

	// Bind Data
	// Bind VB
	GPUContext::ptr context = GPUDevice::Instance()->GetContext();
	context->BindProgram(SphereProgram);
	context->BindUB(0, SphereUniform);
	context->BindSR(0, SphereAssetTex);
	context->BindVAO(SphereVertexBuffer);
	context->DrawElements(GL_TRIANGLES, indexSize, 0u);
}

void EApp::DebugTriangleRun()
{
	// Clear Buffer & Fill Screen
	GPUDevice::Instance()->GetContext()->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GPUDevice::Instance()->GetContext()->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Draw Objects
	VertexLayout tVertexLayout;
	tVertexLayout.Attach<AttrType::POSITION_3D>();
	tVertexLayout.Attach<AttrType::TEXCOORD_2D>();
	tVertexLayout.Attach<AttrType::COLOR_RGB>();

	VertexDataArray tVertexDataArray{ tVertexLayout, 3u };
	tVertexDataArray[0].Attr<AttrType::POSITION_3D>() = { 0.0f, 0.5f, 0.0f };
	tVertexDataArray[1].Attr<AttrType::POSITION_3D>() = { 0.5f, 0.0f, 0.0f };
	tVertexDataArray[2].Attr<AttrType::POSITION_3D>() = { -0.5f, 0.0f, 0.0f };

	tVertexDataArray[0].Attr<AttrType::TEXCOORD_2D>() = { 0.0f, 0.0f };
	tVertexDataArray[1].Attr<AttrType::TEXCOORD_2D>() = { 1.0f, 0.0f };
	tVertexDataArray[2].Attr<AttrType::TEXCOORD_2D>() = { 0.0f, 1.0f };

	tVertexDataArray[0].Attr<AttrType::COLOR_RGB>() = { 1.0f, 0.0f, 0.0f };
	tVertexDataArray[1].Attr<AttrType::COLOR_RGB>() = { 0.0f, 1.0f, 0.0f };
	tVertexDataArray[2].Attr<AttrType::COLOR_RGB>() = { 0.0f, 0.0f, 1.0f };

	// Create VAO Without Index
	VertexBuffer::ptr tVertexBuffer = GPUDevice::Instance()->CreateVertexBuffer("testVAO", tVertexDataArray);
	// Create Shader Program
	GPUProgram::ptr tGPUProgram = GPUDevice::Instance()->CreateGPUProgram("testPrg");
	tGPUProgram->AttachShader(ShaderType::VS, "testTriangle_vs.vert");
	tGPUProgram->AttachShader(ShaderType::PS, "testTriangle_fs.frag");

	// Create Uniform Buffer
	struct TestStruct {
		glm::mat4 modelTrans;
		glm::mat4 viewTrans;
		glm::mat4 projTrans;
	};
	UniformBuffer::ptr tUniformBuffer = GPUDevice::Instance()->CreateUniformBuffer("testUB", GL_STATIC_DRAW, sizeof(TestStruct));
	TestStruct testStruct;
	testStruct.modelTrans = glm::identity<glm::mat4>();
	testStruct.viewTrans = mCamera->getView();
	testStruct.projTrans = mCamera->getProjection();
	tUniformBuffer->FillBuffer(0, sizeof(TestStruct), &testStruct);

	// Create AssetTexture
	GPUSampler::ptr tTexSampler = GPUSampler::Gen2D(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	AssetTexture::ptr tAssetTex = GPUDevice::Instance()->CreateAssetTexture2D("awesomeface.png", tTexSampler);

	// Bind Resources For Rendering
	GPUDevice::Instance()->GetContext()->BindProgram(tGPUProgram);
	GPUDevice::Instance()->GetContext()->BindUB(0, tUniformBuffer);
	GPUDevice::Instance()->GetContext()->BindSR(0, tAssetTex);
	GPUDevice::Instance()->GetContext()->BindVAO(tVertexBuffer);

	GPUDevice::Instance()->GetContext()->DrawArrays(GL_TRIANGLES, 0, 3);
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
