#include "DeferredMaterial.h"
#include "../GPUDevice.h"
#include "../GPUContext.h"
#include "../Shader/GPUProgram.h"
#include "../Buffer/UniformBuffer.h"
#include "../Texture/AssetTexture.h"
#include "../../Render/RenderContext.h"
#include "../../Render/RenderView.h"

struct TransData {
	glm::mat4 modelTrans;
	glm::mat4 viewTrans;
	glm::mat4 projTrans;
};

void DeferredMaterial::Init()
{
	mShaderProgram = GPUDevice::Instance()->CreateGPUProgram("Program_DeferredMaterial");
	mShaderProgram->AttachShader(ShaderType::VS, "Material/DeferredMaterial.vert");
	mShaderProgram->AttachShader(ShaderType::PS, "Material/DeferredMaterial.frag");

	mUniformBuffer = GPUDevice::Instance()->CreateUniformBuffer("UB_DeferredMaterial", GL_STATIC_DRAW, sizeof(TransData));
}

void DeferredMaterial::Bind(BindInfo& bindinfo)
{
	GPUContext::ptr gContext = bindinfo.MyRenderContext->mGPUContext;
	// Bind Program
	gContext->BindProgram(mShaderProgram);
	// Bind SR
	gContext->BindSR(0, mBaseColorTex);
	gContext->BindSR(1, mNormalTex);
	gContext->BindSR(2, mMetallicTex);
	gContext->BindSR(3, mRoughnessTex);
	gContext->BindSR(4, mAoTex);
	// Bind UB
	TransData tempTransData;
	tempTransData.modelTrans = bindinfo.MyDrawCall->MyModelTrans;
	tempTransData.viewTrans = bindinfo.MyRenderContext->mRenderView->MyViewTrans;
	if (bindinfo.MyRenderContext->mRenderView->isProj)
		tempTransData.projTrans = bindinfo.MyRenderContext->mRenderView->MyProjTrans;
	else
		tempTransData.projTrans = bindinfo.MyRenderContext->mRenderView->MyOrthoTrans;

	mUniformBuffer->FillBuffer(0, sizeof(TransData), &tempTransData);
	gContext->BindUB(0, mUniformBuffer);
}
