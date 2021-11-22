#pragma once
#include "Texture/GPUTexture.h"
#include "Buffer/FrameBuffer.h"
#include "Buffer/UniformBuffer.h"
#include "Buffer/VertexBuffer.h"

class GPUDevice;
class GPUProgram;


// GPU上下文，用于发布渲染命令、设置管线状态、绑定渲染资源
class GPUContext {
public:
	GPUContext(GPUDevice* device)
		:mDevice(device) {}

	// Bind Shader Program
	inline void BindProgram(std::shared_ptr<GPUProgram> program) {
		mProgram = program;
	}

	// Bind Shader Resource
	inline void BindSR(unsigned int index, GLenum texType, std::shared_ptr<GPUTexture> texPtr) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(texType, texPtr->GetResourceID());
	}

	// Bind Vertex Array
	inline void BindVAO(std::shared_ptr<VertexBuffer> vao) {
		glBindVertexArray(vao->GetResourceID());
	}

	// Bind Frame Buffer
	inline void BindFrameBuffer(std::shared_ptr<FrameBuffer> fbo) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo->GetResourceID());
	}

	// Draw Without Indices
	inline void DrawArrays(GLenum mode, unsigned int first, unsigned int count) {
		glDrawArrays(mode, first, count);
	}

	// Draw With Indices
	inline void DrawElements(GLenum mode, unsigned int count, unsigned int offset = 0) {
		glDrawElements(mode, count, GL_UNSIGNED_INT, (const void*)offset);
	}

private:
	GPUDevice* mDevice;
	std::shared_ptr<GPUProgram> mProgram;
};