#include "GPUContext.h"

#include "Buffer/FrameBuffer.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/UniformBuffer.h"
#include "Shader/GPUProgram.h"
#include "Texture/GPUTexture.h"

void GPUContext::DrawArrays(GLenum mode, unsigned int first, unsigned int count)
{
	// Bind FrameBuffer
	if (mFrameBuffer != nullptr) {
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer->GetResourceID());
		ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer->GetResourceID());
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	// Activate ShaderProgram
	mProgram->Activate();
	// Bind Uniform Buffer
	for (const auto& i : mUniformBuffers) {
		glBindBufferBase(GL_UNIFORM_BUFFER, i.index, i.uniform->GetResourceID());
	}
	// Bind Shader Resource
	for (const auto& i : mTextures) {
		glActiveTexture(GL_TEXTURE0 + i.index);
		glBindTexture(i.texture->GetTexDesc()->Type, i.texture->GetResourceID());
	}
	// Bind VertexBuffer
	glBindVertexArray(mVertexBuffer->GetResourceID());
	// DrawArrays
	glDrawArrays(mode, first, count);

	// Clean Up
	CleanUp();
}

void GPUContext::DrawElements(GLenum mode, unsigned int count, unsigned int offset)
{
	// Bind FrameBuffer
	if (mFrameBuffer != nullptr) {
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer->GetResourceID());
		ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	// Activate ShaderProgram
	mProgram->Activate();
	// Bind Uniform Buffer
	for (const auto& i : mUniformBuffers) {
		glBindBufferBase(GL_UNIFORM_BUFFER, i.index, i.uniform->GetResourceID());
	}
	// Bind Shader Resource
	for (const auto& i : mTextures) {
		glActiveTexture(GL_TEXTURE0 + i.index);
		glBindTexture(i.texture->GetTexDesc()->Type, i.texture->GetResourceID());
	}
	// Bind VertexBuffer
	glBindVertexArray(mVertexBuffer->GetResourceID());
	// DrawElements
	glDrawElements(mode, count, GL_UNSIGNED_INT, (const void*)offset);

	// Clear Up
	CleanUp();
}
