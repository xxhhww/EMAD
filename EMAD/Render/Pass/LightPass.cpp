#include "LightPass.h"

#include "../../Graphics/GPUDevice.h"
#include "../../Graphics/GPUContext.h"
#include "../../Graphics/Shader/GPUProgram.h"
#include "../../Graphics/Texture/ShaderResource.h"
#include "../../Graphics/Texture/GPUTexture.h"

#include "../RenderContext.h"
#include "../RenderView.h"
#include "../RenderBuffers.h"

#include "../../Geometry/Quad.h"

void LightPass::Init()
{
	// 从"Shader/"中读取LightPass需要使用的着色器
	mShaderProgram = GPUDevice::Instance()->CreateGPUProgram("Program_LightPass");
	mShaderProgram->AttachShader(ShaderType::VS, "LightPass/LightPass.vert");
	mShaderProgram->AttachShader(ShaderType::PS, "LightPass/LightPass.frag");
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
	struct TestShaderData {
		glm::vec3 viewPos;
		glm::vec3 lightColor;
		glm::vec3 lightPos;
	};
	UniformBuffer::ptr ShaderDataUB = GPUDevice::Instance()->CreateUniformBuffer("UB_LightPass", GL_STATIC_DRAW, sizeof(TestShaderData));
	TestShaderData tempShaderData;
	tempShaderData.viewPos = rContext->mRenderView->MyViewPos;
	tempShaderData.lightColor = glm::vec3{ 23.47f, 21.31f, 20.79f };
	tempShaderData.lightPos = glm::vec3{ 0.0f, 0.0f, 3.0f };
	ShaderDataUB->FillBuffer(0, sizeof(TestShaderData), &tempShaderData);

	gContext->BindUB(0, ShaderDataUB);

	// 设置LightPass对应的着色器
	gContext->BindProgram(mShaderProgram);

	// Draw
	Quad::Render(rContext->mGPUContext);
}
