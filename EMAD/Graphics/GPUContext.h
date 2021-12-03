#pragma once

#include <glad/glad.h>
#include <memory>
#include <vector>

class GPUDevice;
class FrameBuffer;
class GPUProgram;
class ShaderResource;
class UniformBuffer;
class VertexBuffer;


// GPU上下文，用于发布渲染命令、设置管线状态、绑定渲染资源
class GPUContext {
public:
	using ptr = std::shared_ptr<GPUContext>;
public:

	GPUContext(GPUDevice* device)
		:mDevice(device) {}

	// 清理屏幕颜色缓存/深度缓存/模板缓存
	inline void ClearBuffer(GLbitfield mask) {
		glClear(mask);
	}
	
	// Clear Color
	inline void ClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	// Bind Shader Program
	inline void BindProgram(std::shared_ptr<GPUProgram> program) {
		mProgram = program;
	}

	// Bind Shader Resource
	inline void BindSR(unsigned int index, std::shared_ptr<ShaderResource> texPtr) {
		/*
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(texType, texPtr->GetResourceID());
		*/
		mTextures.emplace_back(index, texPtr);
	}

	// Bind Vertex Array
	inline void BindVAO(std::shared_ptr<VertexBuffer> vao) {
		// glBindVertexArray(vao->GetResourceID());
		mVertexBuffer = vao;
	}
	
	// Bind Uniform Buffer(将ubo对象ID绑定到着色器程序对应的UniformIndex上)
	inline void BindUB(unsigned int index, std::shared_ptr<UniformBuffer> ubo) {
		// glBindBufferBase(GL_UNIFORM_BUFFER, index, ubo->GetResourceID());
		mUniformBuffers.emplace_back(index, ubo);
	}

	// Bind Frame Buffer
	inline void BindFB(std::shared_ptr<FrameBuffer> fbo) {
		// glBindFramebuffer(GL_FRAMEBUFFER, fbo->GetResourceID());
		mFrameBuffer = fbo;
	}

	// 清理之前所绑定的渲染资源
	inline void CleanUp() {

	}

	// Draw Without Indices
	void DrawArrays(GLenum mode, unsigned int first, unsigned int count);

	// Draw With Indices
	void DrawElements(GLenum mode, unsigned int count, unsigned int offset = 0);
private:
	struct TextureWithIndex {
		unsigned int index;
		std::shared_ptr<ShaderResource> texture;
	};

	struct UniformWithIndex {
		unsigned int index;
		std::shared_ptr<UniformBuffer> uniform;
	};

private:
	GPUDevice* mDevice;

	// 当前渲染帧需要绑定的资源
	// Frame Buffer
	std::shared_ptr<FrameBuffer> mFrameBuffer;
	// Vertex Buffer
	std::shared_ptr<VertexBuffer> mVertexBuffer;
	// Uniform Buffer
	std::vector<UniformWithIndex> mUniformBuffers;
	// GPU Shader Program
	std::shared_ptr<GPUProgram> mProgram;
	// GPU Texture
	std::vector<TextureWithIndex> mTextures;
};