#include "EApp.h"
#include "Object/Camera/Camera.h"
#include <GWin/GWin.h>
#include <Graphics/GPUDevice.h>
#include <Graphics/GPUContext.h>
#include <Graphics/Texture/AssetTexture.h>
#include <Graphics/Buffer/VertexBuffer.h>
#include <Graphics/Buffer/FrameBuffer.h>
#include <Graphics/Buffer/UniformBuffer.h>
#include <Graphics/Shader/GPUProgram.h>
#include <Graphics/Material/DeferredMaterial.h>
#include <Geometry/Sphere.h>
#include <Geometry/Cube.h>
#include <Geometry/Quad.h>
#include <Render/MainRender.h>
#include <Render/DrawCall.h>
#include <Render/RenderView.h>
#include <Render/RenderList.h>
#include <Render/RenderBuffers.h>
#include <Render/RenderContext.h>
#include <Render/Pass/GBufferPass.h>
#include <Render/Pass/LightPass.h>

#include <iostream>

EApp::EApp(int width, int height, const std::string& name){

	// 初始化渲染窗口
	GWin::Instance()->Init(width, height, name);
	// 初始化渲染服务
	RenderService::Instance()->Init();

	// 初始化摄像机
	mCamera = std::make_shared<Camera>();

	// 启动深度缓冲测试
	glEnable(GL_DEPTH_TEST);
	// 启动模板测试
	glEnable(GL_STENCIL_TEST);
}

int EApp::Run()
{
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float lastFrame = 0.0f; // 上一帧的时间

	while (!glfwWindowShouldClose(GWin::Instance()->window()))
	{
		// HandleInput
		float aspect = (float)GWin::Instance()->getRectangle().first / (float)GWin::Instance()->getRectangle().second;
		mCamera->setAspect(aspect);
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		this->HandleInput(deltaTime);
		
		//GPUDevice::Instance()->GetContext()->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//GPUDevice::Instance()->GetContext()->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//DebugTriangleRun();
		//DebugSphereRun();
		DebugDeferredRun();
		//DebugQuadRun();
		//DebugFrameBufferRun();

		glfwSwapBuffers(GWin::Instance()->window());
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

void EApp::DebugDeferredRun()
{
	// 当前帧的渲染上下文
	RenderContext::ptr tempRenderContext = std::make_shared<RenderContext>();
	tempRenderContext->SetGPUContext(GPUDevice::Instance()->GetContext());

	// Create RenderView
	RenderView::ptr tempRenderView = std::make_shared<RenderView>();
	tempRenderView->isProj = true;
	tempRenderView->MyViewPos = mCamera->getPosition();
	tempRenderView->MyViewTrans = mCamera->getView();
	tempRenderView->MyProjTrans = mCamera->getProjection();
	tempRenderContext->SetRenderView(tempRenderView);

	// Create Sphere
	VertexBuffer::ptr SphereVB = Sphere::GetVB();
	unsigned int IndexSize = SphereVB->GetIndexSize();

	// Create Deferred Material
	DeferredMaterial::ptr SphereMaterial = std::make_shared<DeferredMaterial>();
	SphereMaterial->Init();
	// Bind AssetTexture to Material
	AssetTexture::ptr baseColorTex;
	AssetTexture::ptr normalTex;
	AssetTexture::ptr metallicTex;
	AssetTexture::ptr roughnessTex;
	AssetTexture::ptr aoTex;
	bool HasCreatedMaterial = GPUDevice::Instance()->HasGPUResource("IronBaseColor");
	if (!HasCreatedMaterial) {
		GPUSampler::ptr sampler = GPUSampler::Gen2D(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		baseColorTex = GPUDevice::Instance()->Create<AssetTexture>("IronBaseColor");
		baseColorTex->Update("iron/rustediron2_basecolor.png", sampler);

		normalTex = GPUDevice::Instance()->Create<AssetTexture>("IronNormal");
		normalTex->Update("iron/rustediron2_normal.png", sampler);

		metallicTex = GPUDevice::Instance()->Create<AssetTexture>("IronMetallic");
		metallicTex->Update("iron/rustediron2_metallic.png", sampler);

		roughnessTex = GPUDevice::Instance()->Create<AssetTexture>("IronRoughness");
		roughnessTex->Update("iron/rustediron2_roughness.png", sampler);

		aoTex = GPUDevice::Instance()->Create<AssetTexture>("IronAo");
		aoTex->Update("iron/rustediron2_ao.png", sampler);
	}
	else {
		baseColorTex = GPUDevice::Instance()->Get<AssetTexture>("IronBaseColor");
		normalTex = GPUDevice::Instance()->Get<AssetTexture>("IronNormal");
		metallicTex = GPUDevice::Instance()->Get<AssetTexture>("IronMetallic");
		roughnessTex = GPUDevice::Instance()->Get<AssetTexture>("IronRoughness");
		aoTex = GPUDevice::Instance()->Get<AssetTexture>("IronAo");
	}

	SphereMaterial->SetBaseColorTex(baseColorTex);
	SphereMaterial->SetNormalTex(normalTex);
	SphereMaterial->SetMetallicTex(metallicTex);
	SphereMaterial->SetRoughnessTex(roughnessTex);
	SphereMaterial->SetAoTex(aoTex);

	// Create DrawCall
	DrawCall::ptr tempDrawCall = std::make_shared<DrawCall>();
	tempDrawCall->MyGeometryData.Mode = GL_TRIANGLES;
	tempDrawCall->MyGeometryData.StartIndex = 0u;
	tempDrawCall->MyGeometryData.IndexSize = IndexSize;
	tempDrawCall->MyGeometryData.VertexBufferPtr = SphereVB;
	
	tempDrawCall->MyMaterial = SphereMaterial;

	tempDrawCall->MyModelTrans = glm::identity<glm::mat4>();
	tempDrawCall->MyWorldPosition = glm::vec3{ 0.0f, 0.0f, 0.0f };
	tempDrawCall->MyScaling = glm::vec3{ 1.0f, 1.0f, 1.0f };

	// Add Draw Call To RenderContex
	tempRenderContext->AddDrawCall(DrawCallPass::GBuffer, tempDrawCall);

	// Render GBuffer Pass
	GBufferPass::Instance()->Render(tempRenderContext);

	// Render Light Pass
	LightPass::Instance()->Render(tempRenderContext);
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
	GPUProgram::ptr SphereProgram = GPUDevice::Instance()->Create<GPUProgram>("Program_DebugSphere");
	SphereProgram->AttachShader(ShaderType::VS, "Debug/DebugSphere.vert");
	SphereProgram->AttachShader(ShaderType::PS, "Debug/DebugSphere.frag");

	// Create UniformBuffer
	UniformBuffer::ptr SphereUniform;
	bool hasCreateUB = GPUDevice::Instance()->HasGPUResource("UB_DebugSphere");
	if (!hasCreateUB) {
		SphereUniform = GPUDevice::Instance()->Create<UniformBuffer>("UB_DebugSphere");
		SphereUniform->AllocBuffer(sizeof(DebugTrans), GL_STATIC_DRAW);
	}
	SphereUniform = GPUDevice::Instance()->Get<UniformBuffer>("UB_DebugSphere");
	DebugTrans debugTrans;

	glm::mat4 modelTrans{ 1.0f };
	glm::mat4 identity{ 1.0f };
	modelTrans *= glm::translate(identity, glm::vec3{0.0f, 0.0f, 0.0f});
	//modelTrans *= glm::rotate(identity, glm::radians(-90.0f), glm::vec3{ 1.0f, 0.0f, 0.0f });
	//modelTrans *= glm::rotate(identity, 00.0f, glm::vec3{ 0.0f, 1.0f, 0.0f });
	//modelTrans *= glm::rotate(identity, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });

	debugTrans.modelTrans = modelTrans;
	debugTrans.viewTrans = mCamera->getView();
	debugTrans.projTrans = mCamera->getProjection();
	SphereUniform->FillBuffer(0, sizeof(DebugTrans), &debugTrans);

	// Create AssetTexture
	bool HasCreatedAssetTex = GPUDevice::Instance()->HasGPUResource("Earth");
	AssetTexture::ptr SphereAssetTex;
	if (!HasCreatedAssetTex) {
		GPUSampler::ptr SphereTexSampler = GPUSampler::Gen2D(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		SphereAssetTex = GPUDevice::Instance()->Create<AssetTexture>("Earth");
		SphereAssetTex->Update("earth2048.bmp", SphereTexSampler);
	}
	SphereAssetTex = GPUDevice::Instance()->Get<AssetTexture>("Earth");


	// Bind Data
	// Bind VB
	GPUContext::ptr context = GPUDevice::Instance()->GetContext();
	context->BindProgram(SphereProgram);
	context->BindUB(0, SphereUniform);
	context->BindSR(0, SphereAssetTex);
	context->BindVAO(SphereVertexBuffer);
	context->DrawElements(GL_TRIANGLES, indexSize, 0u);
}

void EApp::DebugQuadRun()
{
	GPUDevice::Instance()->GetContext()->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GPUDevice::Instance()->GetContext()->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	GPUProgram::ptr testProgram = GPUDevice::Instance()->Create<GPUProgram>("Program_DebugQuad");
	testProgram->AttachShader(ShaderType::VS, "Debug/DebugQuad.vert");
	testProgram->AttachShader(ShaderType::PS, "Debug/DebugQuad.frag");

	GPUSampler::ptr tTexSampler = GPUSampler::Gen2D(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	AssetTexture::ptr testAssetTex = GPUDevice::Instance()->Create<AssetTexture>("AwesomeFace");
	testAssetTex->Update("awesomeface.png", tTexSampler);

	GPUDevice::Instance()->GetContext()->BindProgram(testProgram);
	GPUDevice::Instance()->GetContext()->BindSR(0, testAssetTex);

	Quad::Render(GPUDevice::Instance()->GetContext());
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
	VertexBuffer::ptr tVertexBuffer = GPUDevice::Instance()->Create<VertexBuffer>("testVAO");
	tVertexBuffer->Update(tVertexDataArray);
	// Create Shader Program
	GPUProgram::ptr tGPUProgram = GPUDevice::Instance()->Create<GPUProgram>("testPrg");
	tGPUProgram->AttachShader(ShaderType::VS, "testTriangle_vs.vert");
	tGPUProgram->AttachShader(ShaderType::PS, "testTriangle_fs.frag");

	// Create Uniform Buffer
	struct TestStruct {
		glm::mat4 modelTrans;
		glm::mat4 viewTrans;
		glm::mat4 projTrans;
	};
	UniformBuffer::ptr tUniformBuffer = GPUDevice::Instance()->Create<UniformBuffer>("testUB");
	tUniformBuffer->AllocBuffer(sizeof(TestStruct), GL_STATIC_DRAW);
	TestStruct testStruct;
	testStruct.modelTrans = glm::identity<glm::mat4>();
	testStruct.viewTrans = mCamera->getView();
	testStruct.projTrans = mCamera->getProjection();
	tUniformBuffer->FillBuffer(0, sizeof(TestStruct), &testStruct);

	// Create AssetTexture
	GPUSampler::ptr tTexSampler = GPUSampler::Gen2D(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	AssetTexture::ptr tAssetTex = GPUDevice::Instance()->Create<AssetTexture>("AwesomeFace");
	tAssetTex->Update("awesomeface.png", tTexSampler);

	// Bind Resources For Rendering
	GPUDevice::Instance()->GetContext()->BindProgram(tGPUProgram);
	GPUDevice::Instance()->GetContext()->BindUB(0, tUniformBuffer);
	GPUDevice::Instance()->GetContext()->BindSR(0, tAssetTex);
	GPUDevice::Instance()->GetContext()->BindVAO(tVertexBuffer);

	GPUDevice::Instance()->GetContext()->DrawArrays(GL_TRIANGLES, 0, 3);
}

void EApp::DebugFrameBufferRun()
{
	int srcWidth = GWin::Instance()->getRectangle().first;
	int srcHeight = GWin::Instance()->getRectangle().second;

	FrameBuffer::ptr testFrameBuffer = GPUDevice::Instance()->Create<FrameBuffer>("FB_DebugFB");

	// Create Color Attachment
	GPUSampler::ptr sampler = GPUSampler::Gen2D(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);

	GPUTexDesc::ptr RGBTexDesc = GPUTexDesc::Gen2D(0, GL_RGB, srcWidth, srcHeight, GL_RGB, GL_FLOAT);
	GPUTexture::ptr testGPUTexture = GPUDevice::Instance()->Create<GPUTexture>("GT_DebugFB");
	testGPUTexture->Update(sampler, RGBTexDesc);

	// Add Attachment
	testFrameBuffer->Activate();
	testFrameBuffer->AttachColor(0, testGPUTexture);
	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, srcWidth, srcHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	testFrameBuffer->InActivate();
	

	// Create FB GPUProgram
	GPUProgram::ptr testFBGPUProgram = GPUDevice::Instance()->Create<GPUProgram>("Program_DebugFB");
	testFBGPUProgram->AttachShader(ShaderType::VS, "Debug/DebugFrameBuffer.vert");
	testFBGPUProgram->AttachShader(ShaderType::PS, "Debug/DebugFrameBuffer.frag");

	// Create SphereVB To Draw
	VertexBuffer::ptr SphereVB = Sphere::GetVB();

	// Create UB
	UniformBuffer::ptr testUniformBuffer = GPUDevice::Instance()->Create<UniformBuffer>("UB_DebugFB");
	testUniformBuffer->AllocBuffer(sizeof(DebugTrans), GL_STATIC_DRAW);
	DebugTrans debugTrans;
	debugTrans.modelTrans = glm::identity<glm::mat4>();
	debugTrans.viewTrans = mCamera->getView();
	debugTrans.projTrans = mCamera->getProjection();
	testUniformBuffer->FillBuffer(0, sizeof(DebugTrans), &debugTrans);

	// Draw to FrameBuffer
	
	GPUContext::ptr context = GPUDevice::Instance()->GetContext();
	
	context->BindFB(testFrameBuffer);
	context->BindProgram(testFBGPUProgram);
	context->BindUB(0, testUniformBuffer);
	context->BindVAO(SphereVB);
	context->DrawElements(GL_TRIANGLES, SphereVB->GetIndexSize(), 0u);
	


	// Draw Attachment to Quad

	// Create Quad Program
	GPUProgram::ptr testQuadProgram = GPUDevice::Instance()->Create<GPUProgram>("Program_DebugFB_Quad");
	testQuadProgram->AttachShader(ShaderType::VS, "Debug/DebugQuad.vert");
	testQuadProgram->AttachShader(ShaderType::PS, "Debug/DebugQuad.frag");

	
	GPUSampler::ptr tTexSampler = GPUSampler::Gen2D(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	AssetTexture::ptr testAssetTex = GPUDevice::Instance()->Create<AssetTexture>("AwesomeFace");
	testAssetTex->Update("awesomeface.png", tTexSampler);

	context->BindFB(nullptr);
	context->BindProgram(testQuadProgram);
	context->BindSR(0, testGPUTexture);
	Quad::Render(context);

	glDeleteRenderbuffers(1, &rboDepth);

}

void EApp::HandleInput(float dt)
{
	// esc为窗口退出键
	if (glfwGetKey(GWin::Instance()->window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(GWin::Instance()->window(), true);

	if (!GWin::Instance()->isCursorEnabled()) {
		// handle input
		if (glfwGetKey(GWin::Instance()->window(), GLFW_KEY_W) == GLFW_PRESS)
			mCamera->translate({ 0.0f, 0.0f, dt });
		if (glfwGetKey(GWin::Instance()->window(), GLFW_KEY_S) == GLFW_PRESS)
			mCamera->translate({ 0.0f, 0.0f, -dt });
		if (glfwGetKey(GWin::Instance()->window(), GLFW_KEY_A) == GLFW_PRESS)
			mCamera->translate({ -dt, 0.0f, 0.0f });
		if (glfwGetKey(GWin::Instance()->window(), GLFW_KEY_D) == GLFW_PRESS)
			mCamera->translate({ dt, 0.0f, 0.0f });

		const auto cursorOffset = GWin::Instance()->getCursorOffset();
		GWin::Instance()->setCursorOffset(0.0f, 0.0f);
		mCamera->rotate(cursorOffset.first, cursorOffset.second);
	}
}
