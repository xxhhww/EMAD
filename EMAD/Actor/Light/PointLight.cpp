#include "PointLight.h"

#include <Render/RenderContext.h>
#include <Render/RenderView.h>
#include <Geometry/Sphere.h>
#include <Graphics/GPUDevice.h>
#include <Graphics/GPUContext.h>
#include <Graphics/Shader/GPUProgram.h>
#include <Graphics/Buffer/VertexBuffer.h>
#include <Graphics/Buffer/UniformBuffer.h>
#include <Graphics/Material/Material.h>
#include <Core/Singleton.h>

// 点光源的固体球材质
class SolidSphereMaterial : public Material {
public:
	struct SolidSphereShaderData {
		glm::mat4 modelTrans;
		glm::mat4 viewTrans;
		glm::mat4 projTrans;
		glm::vec4 lightColor;
	};

	virtual void Init() {
		// 加载着色器程序至GPUDevice中
		GPUProgram::ptr SolidSphereProgram = GPUDevice::Instance()->Create<GPUProgram>("GP_SolidSphere");
		SolidSphereProgram->AttachShader(ShaderType::VS, "Material/SolidSphere.vert");
		SolidSphereProgram->AttachShader(ShaderType::PS, "Material/SolidSphere.frag");

		// 创建UniformBuffer并分配空间
		UniformBuffer::ptr SolidUniformBuffer = GPUDevice::Instance()->Create<UniformBuffer>("UB_SolidSphere");
		SolidUniformBuffer->AllocBuffer(sizeof(SolidSphereShaderData), GL_STATIC_DRAW);
	}

	virtual void Bind(BindInfo& bindInfo) {
		GPUContext::ptr context = bindInfo.MyRenderContext->mGPUContext;
		
		SolidSphereShaderData tempShaderData;
		tempShaderData.modelTrans = bindInfo.MyDrawCall->MyModelTrans;
		tempShaderData.viewTrans = bindInfo.MyRenderContext->mRenderView->MyViewTrans;
		tempShaderData.projTrans = bindInfo.MyRenderContext->mRenderView->MyProjTrans;
		tempShaderData.lightColor = mLightColor;

		UniformBuffer::ptr tempUB = GPUDevice::Instance()->Get<UniformBuffer>("UB_SolidSphere");
		tempUB->FillBuffer(0, sizeof(SolidSphereShaderData), &tempShaderData);

		GPUProgram::ptr tempGP = GPUDevice::Instance()->Get<GPUProgram>("GP_SolidSphere");

		context->BindProgram(tempGP);
		context->BindSB(0, tempUB);
	}

	inline void SetLightColor(glm::vec4 color) {
		mLightColor = color;
	}
private:
	glm::vec4 mLightColor;
};


PointLight::PointLight()
{
	mMaterial = std::make_shared<SolidSphereMaterial>();
	mMaterial->Init();
}

void PointLight::Submit(std::shared_ptr<RenderContext> renderContext) {
	// 1、提交固体球的渲染数据
	DrawCall::ptr SolidSphereDrawCall = std::make_shared<DrawCall>();
	// 模型几何数据
	VertexBuffer::ptr SphereVB = Sphere::GetVB();
	SolidSphereDrawCall->MyGeometryData.VertexBufferPtr = SphereVB;
	SolidSphereDrawCall->MyGeometryData.IndexSize = SphereVB->GetIndexSize();
	SolidSphereDrawCall->MyGeometryData.StartIndex = 0u;
	SolidSphereDrawCall->MyGeometryData.Mode = GL_TRIANGLES;
	// 模型变换矩阵
	mScaling = glm::vec3{ 0.1f, 0.1f, 0.1f };
	SolidSphereDrawCall->MyScaling = mScaling;
	SolidSphereDrawCall->MyWorldPosition = mPosition;
	SolidSphereDrawCall->MyModelTrans = Actor::GenModelTrans();
	// 模型材质
	mMaterial->SetLightColor(glm::vec4{ mLightColor, 1.0f });
	SolidSphereDrawCall->MyMaterial = mMaterial;

	renderContext->mRenderList->AddDrawCall(DrawCallPass::Forward, SolidSphereDrawCall);
	// 2、提交点光源的光数据
	renderContext->mRenderList->mPointLights.emplace_back(
		glm::vec4{ mLightColor, 1.0f }, mPosition, mIntensity
	);

}

void PointLight::GenCtrlGui() {

}