#include "LightPass.h"

#include <Graphics/GPUDevice.h>
#include <Graphics/GPUContext.h>
#include <Graphics/Shader/GPUProgram.h>
#include <Graphics/Texture/ShaderResource.h>
#include <Graphics/Texture/GPUTexture.h>
#include <Graphics/Buffer/UniformBuffer.h>
#include <Render/RenderContext.h>
#include <Render/RenderView.h>
#include <Render/RenderBuffers.h>
#include <Geometry/Quad.h>

struct TestShaderData {
	glm::vec4 viewPos;
	glm::vec4 lightColor;
	glm::vec4 lightPos;
};


LightPass::~LightPass(){
	GPUDevice::Instance()->Release(mShaderProgram);
}

void LightPass::Init(){
	// 从"Shader/"中读取LightPass需要使用的着色器
	mShaderProgram = GPUDevice::Instance()->Create<GPUProgram>("Program_LightPass");
	mShaderProgram->AttachShader(ShaderType::VS, "LightPass/LightPass.vert");
	mShaderProgram->AttachShader(ShaderType::PS, "LightPass/LightPass.frag");

	UniformBuffer::ptr ShaderDataUB = GPUDevice::Instance()->Create<UniformBuffer>("UB_LightPass");
	ShaderDataUB->AllocBuffer(sizeof(TestShaderData), GL_STATIC_DRAW);
}

void LightPass::Render(std::shared_ptr<RenderContext> rContext)
{
	GPUContext::ptr gContext = rContext->mGPUContext;

	// 将GBuffer作为ShaderResource进行绑定
	gContext->BindSR(0, rContext->mRenderBuffers->MyGBuffer.BaseColor);
	gContext->BindSR(1, rContext->mRenderBuffers->MyGBuffer.Position);
	gContext->BindSR(2, rContext->mRenderBuffers->MyGBuffer.Normal);
	gContext->BindSR(3, rContext->mRenderBuffers->MyGBuffer.Other);

	// Bind UB
	TestShaderData tempShaderData;
	tempShaderData.viewPos = glm::vec4(rContext->mRenderView->MyViewPos, 1.0f);
	tempShaderData.lightColor = glm::vec4{ 150.0f, 150.0f, 150.0f, 0.0f };
	tempShaderData.lightPos = glm::vec4{ 0.0f, 0.0f, 10.0f, 0.0f };
	UniformBuffer::ptr tempShaderDataUB = GPUDevice::Instance()->Get<UniformBuffer>("UB_LightPass");
	tempShaderDataUB->FillBuffer(0, sizeof(TestShaderData), &tempShaderData);

	gContext->BindUB(0, tempShaderDataUB);

	// 设置LightPass对应的着色器
	gContext->BindProgram(mShaderProgram);

	// Draw
	Quad::Render(rContext->mGPUContext);
}
