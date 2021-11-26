#pragma once
#include "Shader/GPUProgram.h"
#include "Texture/GPUTexture.h"
#include "Buffer/FrameBuffer.h"
#include "Buffer/UniformBuffer.h"
#include "Buffer/VertexBuffer.h"

class GPUDevice;


// GPU�����ģ����ڷ�����Ⱦ������ù���״̬������Ⱦ��Դ
class GPUContext {
public:
	GPUContext(GPUDevice* device)
		:mDevice(device) {}

	// Clear Color
	inline void FillColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

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

	
	// Bind Uniform Buffer(��ubo����ID�󶨵���ɫ�������Ӧ��UniformIndex��)
	inline void BindUB(unsigned int index, std::shared_ptr<UniformBuffer> ubo) {
		glBindBufferBase(GL_UNIFORM_BUFFER, index, ubo->GetResourceID());
	}

	// Bind Frame Buffer
	inline void BindFB(std::shared_ptr<FrameBuffer> fbo) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo->GetResourceID());
	}

	// Draw Without Indices
	inline void DrawArrays(GLenum mode, unsigned int first, unsigned int count) {
		mProgram->Activate();
		glDrawArrays(mode, first, count);
	}

	// Draw With Indices
	inline void DrawElements(GLenum mode, unsigned int count, unsigned int offset = 0) {
		mProgram->Activate();
		glDrawElements(mode, count, GL_UNSIGNED_INT, (const void*)offset);
	}

private:
	GPUDevice* mDevice;
	// ��ǰ��Ⱦ֡��Ҫ�󶨵���Դ
	// GPU Shader Program
	std::shared_ptr<GPUProgram> mProgram;
};