#include "GBufferPass.h"

#include <Graphics/GPUDevice.h>
#include <Graphics/GPUContext.h>
#include <Graphics/Buffer/FrameBuffer.h>
#include <Render/RenderContext.h>
#include <Render/RenderBuffers.h>

// Just Test To Include
#include <Graphics/Shader/GPUProgram.h>
#include <Graphics/Texture/AssetTexture.h>
#include <Geometry/Quad.h>
#include <GWin/GWin.h>
#include <iostream>

void GBufferPass::Init(){
	GPUDevice::Instance()->Create<FrameBuffer>("FB_GBufferPass");
}

void GBufferPass::Render(std::shared_ptr<RenderContext> rContext)
{
	// 设置GBuffer所使用的FrameBuffer
	FrameBuffer::ptr tempFB = GPUDevice::Instance()->Get<FrameBuffer>("FB_GBufferPass");
	tempFB->Activate();
	tempFB->AttachColor(0, rContext->mRenderBuffers->MyGBuffer.BaseColor);
	tempFB->AttachColor(1, rContext->mRenderBuffers->MyGBuffer.Position);
	tempFB->AttachColor(2, rContext->mRenderBuffers->MyGBuffer.Normal);
	tempFB->AttachColor(3, rContext->mRenderBuffers->MyGBuffer.Other);
	tempFB->AttachDepth(rContext->mRenderBuffers->DepthBuffer);

	// 告诉OpenGL我们将要使用(帧缓冲的)哪种颜色附件来进行渲染
	GLuint attachments[4] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
	};
	glDrawBuffers(4, attachments);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	tempFB->InActivate();
	
	GPUContext::ptr gContext = rContext->mGPUContext;
	
	// 设置GPU上下文的FrameBuffer
	gContext->BindFB(tempFB);
	// 
	gContext->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// 发布渲染命令，来填充RenderBuffers中的GBuffer
	rContext->ExecDrawCall(DrawCallPass::GBuffer);
	//glDeleteRenderbuffers(1, &rboDepth);
	// 恢复默认帧缓存
	gContext->BindFB(nullptr);

	gContext->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Just Test
	GPUProgram::ptr testProgram = GPUDevice::Instance()->Create<GPUProgram>("Program_DebugGBufferQuad");
	testProgram->AttachShader(ShaderType::VS, "Debug/DebugQuad.vert");
	testProgram->AttachShader(ShaderType::PS, "Debug/DebugQuad.frag");
	gContext->BindProgram(testProgram);
	gContext->BindSR(0, rContext->mRenderBuffers->MyGBuffer.BaseColor);

	Quad::Render(gContext);
	// TODO恢复默认FrameBuffer
	
}


