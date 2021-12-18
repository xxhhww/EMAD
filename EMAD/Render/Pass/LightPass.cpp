#include "LightPass.h"

#include <Graphics/GPUDevice.h>
#include <Graphics/GPUContext.h>
#include <Graphics/Shader/GPUProgram.h>
#include <Graphics/Texture/ShaderResource.h>
#include <Graphics/Texture/GPUTexture.h>
#include <Graphics/Buffer/StorageBuffer.h>
#include <Render/RenderContext.h>
#include <Render/RenderView.h>
#include <Render/RenderBuffers.h>
#include <Geometry/Quad.h>

LightPass::~LightPass(){
	GPUDevice::Instance()->Release(mShaderProgram);
}

void LightPass::Init(){
	// 从"Shader/"中读取LightPass需要使用的着色器
	mShaderProgram = GPUDevice::Instance()->Create<GPUProgram>("Program_LightPass");
	mShaderProgram->AttachShader(ShaderType::VS, "LightPass/LightPass.vert");
	mShaderProgram->AttachShader(ShaderType::PS, "LightPass/LightPass.frag");

	//UniformBuffer::ptr ShaderDataUB = GPUDevice::Instance()->Create<UniformBuffer>("UB_LightPass");
	//ShaderDataUB->AllocBuffer(sizeof(TestShaderData), GL_STATIC_DRAW);
}

void LightPass::Render(std::shared_ptr<RenderContext> rContext)
{
	GPUContext::ptr gContext = rContext->mGPUContext;

	// 将GBuffer作为ShaderResource进行绑定
	gContext->BindSR(0, rContext->mRenderBuffers->MyGBuffer.BaseColor);
	gContext->BindSR(1, rContext->mRenderBuffers->MyGBuffer.Position);
	gContext->BindSR(2, rContext->mRenderBuffers->MyGBuffer.Normal);
	gContext->BindSR(3, rContext->mRenderBuffers->MyGBuffer.Other);

	// Bind UB(for pointLights)
	StorageBuffer::ptr tempPointLightSB = GPUDevice::Instance()->Create<StorageBuffer>("SB_LightPass_PL");
	int size = rContext->mRenderList->mPointLights.size();
	tempPointLightSB->AllocBuffer(sizeof(glm::vec3) + sizeof(int) + size * sizeof(PointLightData), GL_STATIC_DRAW);
	tempPointLightSB->FillBuffer(0, sizeof(glm::vec3), &rContext->mRenderView->MyViewPos);
	tempPointLightSB->FillBuffer(sizeof(glm::vec3), sizeof(int), &size);
	tempPointLightSB->FillBuffer(sizeof(glm::vec3) + sizeof(int), size * sizeof(PointLightData), rContext->mRenderList->mPointLights.data());

	gContext->BindSB(0, tempPointLightSB);

	// 设置LightPass对应的着色器
	gContext->BindProgram(mShaderProgram);
	// 
	gContext->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Draw
	Quad::Render(rContext->mGPUContext);
}
