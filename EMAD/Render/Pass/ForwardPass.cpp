#include "ForwardPass.h"

#include <GWin/GWin.h>
#include <Graphics/GPUDevice.h>
#include <Graphics/Buffer/FrameBuffer.h>
#include <Render/RenderContext.h>

void ForwardPass::Init()
{
}

void ForwardPass::Render(std::shared_ptr<RenderContext> rContext)
{
	int srcWidth = GWin::Instance()->getRectangle().first;
	int srcHeight = GWin::Instance()->getRectangle().second;

	FrameBuffer::ptr GBufferFB = GPUDevice::Instance()->Get<FrameBuffer>("FB_GBufferPass");
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GBufferFB->GetResourceID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Ð´Èëµ½Ä¬ÈÏÖ¡»º³å
	glBlitFramebuffer(
		0, 0, srcWidth, srcHeight, 0, 0, srcWidth, srcHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);

	// ·¢²¼äÖÈ¾ÃüÁî£¬
	rContext->ExecDrawCall(DrawCallPass::Forward);
}
