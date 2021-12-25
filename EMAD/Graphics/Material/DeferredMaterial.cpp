#include "DeferredMaterial.h"
#include "../GPUDevice.h"
#include "../GPUContext.h"
#include "../Shader/GPUProgram.h"
#include "../Buffer/UniformBuffer.h"
#include "../Texture/AssetTexture.h"
#include "../../Render/RenderContext.h"
#include "../../Render/RenderView.h"

#include <ImGui/imgui.h>

#include <sstream>

struct TransData {
	glm::mat4 modelTrans;
	glm::mat4 viewTrans;
	glm::mat4 projTrans;
};

void DeferredMaterial::Init()
{
	mShaderProgram = GPUDevice::Instance()->Create<GPUProgram>("Program_DeferredMaterial");
	mShaderProgram->ReComplie();
	mShaderProgram->AttachShader(ShaderType::VS, "Material/DeferredMaterial.vert");
	mShaderProgram->AttachShaderEx(ShaderType::PS, GenShaderCode());

	if (!mShaderBuffer) {
		mShaderBuffer = GPUDevice::Instance()->Create<UniformBuffer>("UB_DeferredMaterial");
		mShaderBuffer->AllocBuffer(sizeof(TransData), GL_STATIC_DRAW);
	}
}

void DeferredMaterial::Bind(BindInfo& bindinfo)
{
	GPUContext::ptr gContext = bindinfo.MyRenderContext->mGPUContext;
	// Bind Program
	gContext->BindProgram(mShaderProgram);

	mShaderProgram->Activate();
	// Bind SR
	unsigned int tempIndex = 0;
	if (mHasBaseColorTex) {
		gContext->BindSR(tempIndex, mBaseColorTex);
		tempIndex++;
	}
	else {
		mShaderProgram->setVec3("BaseColor", mBaseColor);
	}
	if (mHasNormalTex) {
		gContext->BindSR(tempIndex, mNormalTex);
		tempIndex++;
	}
	if (mHasMetallicTex) {
		gContext->BindSR(tempIndex, mMetallicTex);
		tempIndex++;
	}
	else {
		mShaderProgram->setFloat("Metallic", mMetallic);
	}
	if (mHasRoughnessTex) {
		gContext->BindSR(tempIndex, mRoughnessTex);
		tempIndex;
	}
	else {
		mShaderProgram->setFloat("Roughness", mRoughness);
	}
	if (mHasAoTex) {
		gContext->BindSR(tempIndex, mAoTex);
		tempIndex++;
	}
	else {
		mShaderProgram->setFloat("Ao", mAo);
	}

	// Bind UB
	TransData tempTransData;
	tempTransData.modelTrans = bindinfo.MyDrawCall->MyModelTrans;
	tempTransData.viewTrans = bindinfo.MyRenderContext->mRenderView->MyViewTrans;
	if (bindinfo.MyRenderContext->mRenderView->isProj)
		tempTransData.projTrans = bindinfo.MyRenderContext->mRenderView->MyProjTrans;
	else
		tempTransData.projTrans = bindinfo.MyRenderContext->mRenderView->MyOrthoTrans;

	mShaderBuffer->FillBuffer(0, sizeof(TransData), &tempTransData);
	gContext->BindSB(0, mShaderBuffer);
}

static std::string DeferredShaderHeader =
	"#version 450 core\n"
	"layout(location = 0) out vec3 GBaseColor;\n"
	"layout(location = 1) out vec3 GPosition;\n"
	"layout(location = 2) out vec3 GNormal;\n"
	"layout(location = 3) out vec3 GOther;\n"
	"\n"
	"in VSOut{\n"
		"in vec3 fPosition;\n"
		"in vec2 fTexcoord;\n"
		"in vec3 fNormal;\n"
	"} fsin;\n";


void DeferredMaterial::GenCtrlGui()
{
	if (ImGui::Begin("TestMaterial")) {
		ImGui::ColorEdit3("BaseColor", &mBaseColor.x);

		ImGui::SliderFloat("Metallic", &mMetallic, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("Roughness", &mRoughness, 0.0f, 1.0f, "%.1f");
	}
	ImGui::End();
}

std::string DeferredMaterial::GenShaderCode()
{
	unsigned int tempBindIndex = 0;
	std::stringstream ss;

	ss << DeferredShaderHeader;

	if (mHasBaseColorTex) {
		ss << "layout(binding = " << tempBindIndex << ")" << "uniform sampler2D";
		ss << "BaseColor;";
		ss << "\n";

		tempBindIndex++;
	}
	else {
		//ss << "vec3 BaseColor = vec3(" << mBaseColor.r << "f, " << mBaseColor.g << "f, " << mBaseColor.b << "f);\n";
		ss << "uniform vec3 BaseColor;\n";
	}

	if (mHasNormalTex) {
		ss << "layout(binding = " << tempBindIndex << ")" << "uniform sampler2D";
		ss << "Normal;";
		ss << "\n";

		tempBindIndex++;
	}
	else {
		// ss << "vec3 Normal = vec3(" << mNormal.r << "," << mNormal.g << "," << mNormal.b << ");\n";
	}

	if (mHasMetallicTex) {
		ss << "layout(binding = " << tempBindIndex << ")" << "uniform sampler2D";
		ss << "Metallic;";
		ss << "\n";

		tempBindIndex++;
	}
	else {
		// ss << "float Metallic = " << mMetallic << "f;\n";
		ss << "uniform float Metallic;\n";
	}

	if (mHasRoughnessTex) {
		ss << "layout(binding = " << tempBindIndex << ")" << "uniform sampler2D";
		ss << "Roughness;";
		ss << "\n";

		tempBindIndex++;
	}
	else {
		// ss << "float Roughness = " << mRoughness << "f;\n";
		ss << "uniform float Roughness;\n";
	}

	if (mHasAoTex) {
		ss << "layout(binding = " << tempBindIndex << ")" << "uniform sampler2D";
		ss << "Ao;";
		ss << "\n";

		tempBindIndex++;
	}
	else {
		// ss << "float Ao = " << mAo << "f;\n";
		ss << "uniform float Ao;\n";
	}

	ss << "void main(){\n";


	if (mHasBaseColorTex) {
		ss << "GBaseColor = texture(BaseColor, fsin.fTexcoord).rgb;\n";
	}
	else {
		ss << "GBaseColor = BaseColor;\n";
	}

	if (mHasNormalTex) {
		ss << "GNormal = texture(Normal, fsin.fTexcoord).rgb;\n";
	}
	else {
		ss << "GNormal = fsin.fNormal;\n";
	}

	if (mHasMetallicTex) {
		ss << "GOther.r = texture(Metallic, fsin.fTexcoord).r;\n";
	}
	else {
		ss << "GOther.r = Metallic;\n";
	}

	if (mHasRoughnessTex) {
		ss << "GOther.g = texture(Roughness, fsin.fTexcoord).r;\n";
	}
	else {
		ss << "GOther.g = Roughness;\n";
	}

	if (mHasAoTex) {
		ss << "GOther.b = texture(Ao, fsin.fTexcoord).r;\n";
	}
	else {
		ss << "GOther.b = Ao;\n";
	}

	ss << "GPosition = fsin.fPosition;\n";

	ss << "}";

	return ss.str();
}
