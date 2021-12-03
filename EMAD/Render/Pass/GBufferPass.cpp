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
		// 设置GBuffer所使用的FrameBuffer
		mFrameBuffer = GPUDevice::Instance()->CreateFrameBuffer("FB_GBufferPass");
		mFrameBuffer->AttachColor(0, rContext->mRenderBuffers->MyGBuffer.BaseColor);
		mFrameBuffer->AttachColor(1, rContext->mRenderBuffers->MyGBuffer.Position);
		mFrameBuffer->AttachColor(2, rContext->mRenderBuffers->MyGBuffer.Normal);
		mFrameBuffer->AttachColor(3, rContext->mRenderBuffers->MyGBuffer.Other);

		// 告诉OpenGL我们将要使用(帧缓冲的)哪种颜色附件来进行渲染
		GLuint attachments[4] = { 
			GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
		};
		glDrawBuffers(4, attachments);
	}

	GPUContext::ptr gContext = rContext->mGPUContext;
	
	// 设置GPU上下文的FrameBuffer
	gContext->BindFB(mFrameBuffer);
	// 发布渲染命令，来填充RenderBuffers中的GBuffer
	rContext->ExecDrawCall(DrawCallPass::GBuffer);
	// TODO恢复默认FrameBuffer

}


