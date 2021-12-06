#include "RenderBuffers.h"
#include <GWin/GWin.h>
#include <Graphics/GPUDevice.h>
#include <Graphics/Texture/GPUTexture.h>

RenderBuffers::RenderBuffers()
{
	GPUSampler::ptr sampler = GPUSampler::Gen2D(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);

	int srcWidth = GWin::Instance()->getRectangle().first;
	int srcHeight = GWin::Instance()->getRectangle().second;

	GPUTexDesc::ptr RGBTexDesc = GPUTexDesc::Gen2D(0, GL_RGB, srcWidth, srcHeight, GL_RGB, GL_FLOAT);
	GPUTexDesc::ptr RGB16FTexDesc = GPUTexDesc::Gen2D(0, GL_RGB16F, srcWidth, srcHeight, GL_RGB, GL_FLOAT);

	// 为了方便，此处资源的创建不经过GPUDevice，而是直接创建
	MyGBuffer.BaseColor = GPUDevice::Instance()->CreateGPUTexture("GT_GBufferColor", sampler, RGBTexDesc);
	MyGBuffer.Position = GPUDevice::Instance()->CreateGPUTexture("GT_GBufferPosition", sampler, RGB16FTexDesc);
	MyGBuffer.Normal = GPUDevice::Instance()->CreateGPUTexture("GT_GBufferNormal", sampler, RGB16FTexDesc);
	MyGBuffer.Other = GPUDevice::Instance()->CreateGPUTexture("GT_GBufferOther", sampler, RGBTexDesc);
}

RenderBuffers::~RenderBuffers()
{
	// 由于所有的GPUResource都是由GPUDevice管理的
	// 所以，当RenderBuffers析构后，其资源仍然会存在于GPUDevice上
	// 因此，需要通知GPUDevice进行析构
}
