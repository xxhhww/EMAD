#include "GBufferPass.h"

#include "../../Graphics/GPUDevice.h"
#include "../../Graphics/GPUContext.h"
#include "../../Graphics/Buffer/FrameBuffer.h"

#include "../RenderContext.h"
#include "../RenderBuffers.h"

// Just Test To Include
#include <Graphics/Shader/GPUProgram.h>
#include <Graphics/Texture/AssetTexture.h>
#include <Geometry/Quad.h>
#include <GWin/GWin.h>
#include <iostream>

void GBufferPass::Init(){
	mFrameBuffer = nullptr;
}

void GBufferPass::Render(std::shared_ptr<RenderContext> rContext)
{
	// 设置GBuffer所使用的FrameBuffer
	FrameBuffer::ptr tempFB = GPUDevice::Instance()->CreateFrameBuffer("FB_GBufferPass");
	tempFB->Activate();
	tempFB->AttachColor(0, rContext->mRenderBuffers->MyGBuffer.BaseColor);
	tempFB->AttachColor(1, rContext->mRenderBuffers->MyGBuffer.Position);
	tempFB->AttachColor(2, rContext->mRenderBuffers->MyGBuffer.Normal);
	tempFB->AttachColor(3, rContext->mRenderBuffers->MyGBuffer.Other);
	// 告诉OpenGL我们将要使用(帧缓冲的)哪种颜色附件来进行渲染
	GLuint attachments[4] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
	};
	glDrawBuffers(4, attachments);

	int srcWidth = GWin::Instance()->getRectangle().first;
	int srcHeight = GWin::Instance()->getRectangle().second;
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, srcWidth, srcHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	tempFB->InActivate();
	
	GPUContext::ptr gContext = rContext->mGPUContext;
	
	// 设置GPU上下文的FrameBuffer
	gContext->BindFB(tempFB);
	// 发布渲染命令，来填充RenderBuffers中的GBuffer
	rContext->ExecDrawCall(DrawCallPass::GBuffer);

	/*
	// Just Test
	gContext->BindFB(nullptr);
	GPUProgram::ptr testProgram = GPUDevice::Instance()->CreateGPUProgram("Program_DebugGBufferQuad");
	testProgram->AttachShader(ShaderType::VS, "Debug/DebugQuad.vert");
	testProgram->AttachShader(ShaderType::PS, "Debug/DebugQuad.frag");

	GPUSampler::ptr tTexSampler = GPUSampler::Gen2D(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	AssetTexture::ptr testAssetTex = GPUDevice::Instance()->CreateAssetTexture2D("awesomeface.png", tTexSampler);

	gContext->BindProgram(testProgram);
	gContext->BindSR(0, rContext->mRenderBuffers->MyGBuffer.BaseColor);

	Quad::Render(gContext);
	// TODO恢复默认FrameBuffer
	*/
}


