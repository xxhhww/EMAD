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

	// Ϊ�˷��㣬�˴���Դ�Ĵ���������GPUDevice������ֱ�Ӵ���
	MyGBuffer.BaseColor = GPUDevice::Instance()->CreateGPUTexture("GT_GBufferColor", sampler, RGBTexDesc);
	MyGBuffer.Position = GPUDevice::Instance()->CreateGPUTexture("GT_GBufferPosition", sampler, RGB16FTexDesc);
	MyGBuffer.Normal = GPUDevice::Instance()->CreateGPUTexture("GT_GBufferNormal", sampler, RGB16FTexDesc);
	MyGBuffer.Other = GPUDevice::Instance()->CreateGPUTexture("GT_GBufferOther", sampler, RGBTexDesc);
}

RenderBuffers::~RenderBuffers()
{
	// �������е�GPUResource������GPUDevice�����
	// ���ԣ���RenderBuffers����������Դ��Ȼ�������GPUDevice��
	// ��ˣ���Ҫ֪ͨGPUDevice��������
}
