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

	// ������������Դ
	MyGBuffer.BaseColor = GPUDevice::Instance()->Create<GPUTexture>("GT_GBufferColor");
	MyGBuffer.BaseColor->Update(sampler, RGBTexDesc);

	MyGBuffer.Position = GPUDevice::Instance()->Create<GPUTexture>("GT_GBufferPosition");
	MyGBuffer.Position->Update(sampler, RGB16FTexDesc);

	MyGBuffer.Normal = GPUDevice::Instance()->Create<GPUTexture>("GT_GBufferNormal");
	MyGBuffer.Normal->Update(sampler, RGBTexDesc);

	MyGBuffer.Other = GPUDevice::Instance()->Create<GPUTexture>("GT_GBufferOther");
	MyGBuffer.Other->Update(sampler, RGBTexDesc);

	// ��¼��Ļ���
	MyWidth = srcWidth;
	MyHeight = srcHeight;
}

RenderBuffers::~RenderBuffers()
{
	// �������е�GPUResource������GPUDevice�����
	// ���ԣ���RenderBuffers����������Դ��Ȼ�������GPUDevice��
	// ��ˣ���Ҫ֪ͨGPUDevice��������
	GPUDevice::Instance()->Release(MyGBuffer.BaseColor);
	GPUDevice::Instance()->Release(MyGBuffer.Position);
	GPUDevice::Instance()->Release(MyGBuffer.Normal);
	GPUDevice::Instance()->Release(MyGBuffer.Other);
}

void RenderBuffers::Update(unsigned int Width, unsigned int Height)
{
	// ��Ļ��߷����仯
	if (Width != MyWidth || Height != MyHeight) {
		GPUSampler::ptr sampler = MyGBuffer.BaseColor->GetSampler();
		GPUTexDesc::ptr RGBTexDesc = MyGBuffer.BaseColor->GetTexDesc();
		GPUTexDesc::ptr RGB16FTexDesc = MyGBuffer.Position->GetTexDesc();
		RGBTexDesc->Width = Width;
		RGB16FTexDesc->Height = Height;

		// ����
		MyGBuffer.BaseColor->Update(sampler, RGBTexDesc);
		MyGBuffer.Position->Update(sampler, RGB16FTexDesc);
		MyGBuffer.Normal->Update(sampler, RGBTexDesc);
		MyGBuffer.Other->Update(sampler, RGBTexDesc);

		MyWidth = Width;
		MyHeight = Height;
	}
}
