#include "RenderBuffers.h"
#include <GWin/GWin.h>
#include <Graphics/GPUDevice.h>
#include <Graphics/Texture/GPUTexture.h>

RenderBuffers::RenderBuffers(){
	GPUSampler::ptr sampler = GPUSampler::Gen2D(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);

	int srcWidth = GWin::Instance()->getRectangle().first;
	int srcHeight = GWin::Instance()->getRectangle().second;

	GPUTexDesc::ptr RGBTexDesc = GPUTexDesc::Gen2D(0, GL_RGB, srcWidth, srcHeight, GL_RGB, GL_FLOAT);
	GPUTexDesc::ptr RGB16FTexDesc = GPUTexDesc::Gen2D(0, GL_RGB16F, srcWidth, srcHeight, GL_RGB, GL_FLOAT);

	// 创建并描述资源
	MyGBuffer.BaseColor = GPUDevice::Instance()->Create<GPUTexture>("GT_GBufferColor");
	MyGBuffer.BaseColor->Update(sampler, RGBTexDesc);

	MyGBuffer.Position = GPUDevice::Instance()->Create<GPUTexture>("GT_GBufferPosition");
	MyGBuffer.Position->Update(sampler, RGB16FTexDesc);

	MyGBuffer.Normal = GPUDevice::Instance()->Create<GPUTexture>("GT_GBufferNormal");
	MyGBuffer.Normal->Update(sampler, RGBTexDesc);

	MyGBuffer.Other = GPUDevice::Instance()->Create<GPUTexture>("GT_GBufferOther");
	MyGBuffer.Other->Update(sampler, RGBTexDesc);

	// 记录屏幕宽高
	MyWidth = srcWidth;
	MyHeight = srcHeight;
}

RenderBuffers::~RenderBuffers()
{
	// 由于所有的GPUResource都是由GPUDevice管理的
	// 所以，当RenderBuffers析构后，其资源仍然会存在于GPUDevice上
	// 因此，需要通知GPUDevice进行析构
	GPUDevice::Instance()->Release(MyGBuffer.BaseColor);
	GPUDevice::Instance()->Release(MyGBuffer.Position);
	GPUDevice::Instance()->Release(MyGBuffer.Normal);
	GPUDevice::Instance()->Release(MyGBuffer.Other);
}

void RenderBuffers::Update(unsigned int Width, unsigned int Height)
{
	// 屏幕宽高发生变化
	if (Width != MyWidth || Height != MyHeight) {
		GPUSampler::ptr sampler = MyGBuffer.BaseColor->GetSampler();
		GPUTexDesc::ptr RGBTexDesc = MyGBuffer.BaseColor->GetTexDesc();
		GPUTexDesc::ptr RGB16FTexDesc = MyGBuffer.Position->GetTexDesc();
		RGBTexDesc->Width = Width;
		RGB16FTexDesc->Height = Height;

		// 更新
		MyGBuffer.BaseColor->Update(sampler, RGBTexDesc);
		MyGBuffer.Position->Update(sampler, RGB16FTexDesc);
		MyGBuffer.Normal->Update(sampler, RGBTexDesc);
		MyGBuffer.Other->Update(sampler, RGBTexDesc);

		MyWidth = Width;
		MyHeight = Height;
	}
}
