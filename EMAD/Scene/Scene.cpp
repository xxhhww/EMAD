#include "Scene.h"

#include <Actor/Light/PointLight.h>
#include <Actor/Solid/Ball.h>

#include <Graphics/GPUDevice.h>
#include <Graphics/Material/DeferredMaterial.h>
#include <Graphics/Texture/AssetTexture.h>

#include <Render/RenderContext.h>
#include <Render/RenderView.h>
#include <Render/Pass/GBufferPass.h>
#include <Render/Pass/LightPass.h>
#include <Render/Pass/ForwardPass.h>

#include <Object/Camera/Camera.h>

void Scene::Init()
{
	// 初始化摄像机
	// mCamera = std::make_shared<Camera>();

	// 预设材质
	// Bind AssetTexture to Material
	/*
	static DeferredMaterial::ptr IronMaterial = std::make_shared<DeferredMaterial>();
	static bool HasCteatedMaterial = false;
	if (!HasCteatedMaterial) {
		IronMaterial->Init();

		GPUSampler::ptr sampler = GPUSampler::Gen2D(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		AssetTexture::ptr baseColorTex = GPUDevice::Instance()->Create<AssetTexture>("IronBaseColor");
		baseColorTex->Update("iron/rustediron2_basecolor.png", sampler);

		AssetTexture::ptr normalTex = GPUDevice::Instance()->Create<AssetTexture>("IronNormal");
		normalTex->Update("iron/rustediron2_normal.png", sampler);

		AssetTexture::ptr metallicTex = GPUDevice::Instance()->Create<AssetTexture>("IronMetallic");
		metallicTex->Update("iron/rustediron2_metallic.png", sampler);

		AssetTexture::ptr roughnessTex = GPUDevice::Instance()->Create<AssetTexture>("IronRoughness");
		roughnessTex->Update("iron/rustediron2_roughness.png", sampler);

		AssetTexture::ptr aoTex = GPUDevice::Instance()->Create<AssetTexture>("IronAo");
		aoTex->Update("iron/rustediron2_ao.png", sampler);

		IronMaterial->SetBaseColorTex(baseColorTex);
		IronMaterial->SetNormalTex(normalTex);
		IronMaterial->SetMetallicTex(metallicTex);
		IronMaterial->SetRoughnessTex(roughnessTex);
		IronMaterial->SetAoTex(aoTex);

		HasCteatedMaterial = true;
	}
	*/

	// 初始化场景
	// 点光源
	PointLight::ptr tempPointLight = std::make_shared<PointLight>();
	tempPointLight->SetPosition(0.0f, 0.0, 3.0f);

	mActors.emplace_back(tempPointLight);
	// 金属球
	Ball::ptr tempBall = std::make_shared<Ball>();
	DeferredMaterial::ptr tempMaterial = std::make_shared<DeferredMaterial>();
	tempMaterial->Init();
	tempBall->SetMaterial(tempMaterial);

	mActors.emplace_back(tempBall);
}

void Scene::Render(std::shared_ptr<Camera> camera)
{
	// 初始化渲染上下文
	RenderContext::ptr tempRenderContext = std::make_shared<RenderContext>();
	tempRenderContext->mGPUContext = GPUDevice::Instance()->GetContext();

	// Set RenderView
	RenderView::ptr tempRenderView = std::make_shared<RenderView>();
	tempRenderView->isProj = true;
	tempRenderView->MyViewPos = camera->getPosition();
	tempRenderView->MyViewTrans = camera->getView();
	tempRenderView->MyProjTrans = camera->getProjection();
	tempRenderContext->SetRenderView(tempRenderView);

	for (auto& i : mActors) {
		i->Submit(tempRenderContext);
	}

	GBufferPass::Instance()->Render(tempRenderContext);
	LightPass::Instance()->Render(tempRenderContext);
	ForwardPass::Instance()->Render(tempRenderContext);
	// tempRenderContext->ClearDrawCall();
}

void Scene::GenCtrlGui()
{
	for (const auto& i : mActors) {
		i->GenCtrlGui();
	}
}
