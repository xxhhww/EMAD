#include "GBufferPass.h"

#include "../../Graphics/GPUDevice.h"
#include "../../Graphics/GPUContext.h"
#include "../../Graphics/Buffer/FrameBuffer.h"

#include "../RenderContext.h"
#include "../RenderBuffers.h"

void GBufferPass::Init(){
}

void GBufferPass::Render(std::shared_ptr<RenderContext> rContext)
{
	if (mFrameBuffer == nullptr) {
		// ����GBuffer��ʹ�õ�FrameBuffer
		mFrameBuffer = GPUDevice::Instance()->CreateFrameBuffer("FB_GBufferPass");
		mFrameBuffer->AttachColor(0, rContext->mRenderBuffers->MyGBuffer.BaseColor);
		mFrameBuffer->AttachColor(1, rContext->mRenderBuffers->MyGBuffer.Position);
		mFrameBuffer->AttachColor(2, rContext->mRenderBuffers->MyGBuffer.Normal);
		mFrameBuffer->AttachColor(3, rContext->mRenderBuffers->MyGBuffer.Other);

		// ����OpenGL���ǽ�Ҫʹ��(֡�����)������ɫ������������Ⱦ
		GLuint attachments[4] = { 
			GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
		};
		glDrawBuffers(4, attachments);
	}

	GPUContext::ptr gContext = rContext->mGPUContext;
	
	// ����GPU�����ĵ�FrameBuffer
	gContext->BindFB(mFrameBuffer);
	// ������Ⱦ��������RenderBuffers�е�GBuffer
	rContext->ExecDrawCall(DrawCallPass::GBuffer);
	// TODO�ָ�Ĭ��FrameBuffer

}


