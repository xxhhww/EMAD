#pragma once

#include <glad/glad.h>
#include <memory>
#include <vector>

class GPUDevice;
class FrameBuffer;
class GPUProgram;
class ShaderResource;
class ShaderBuffer;
class VertexBuffer;


// GPU�����ģ����ڷ�����Ⱦ������ù���״̬������Ⱦ��Դ
class GPUContext {
public:
	using ptr = std::shared_ptr<GPUContext>;
public:

	GPUContext(GPUDevice* device)
		:mDevice(device) {}

	// ������Ļ��ɫ����/��Ȼ���/ģ�建��
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
	
	// Bind Shader Buffer(��ubo����ID�󶨵���ɫ�������Ӧ��UniformIndex��)
	inline void BindSB(unsigned int index, std::shared_ptr<ShaderBuffer> sbo) {
		// glBindBufferBase(GL_UNIFORM_BUFFER, index, ubo->GetResourceID());
		mShaderBuffers.emplace_back(index, sbo);
	}

	// Bind Frame Buffer
	void BindFB(std::shared_ptr<FrameBuffer> fbo);

	// ����֮ǰ���󶨵���Ⱦ��Դ
	inline void CleanUp() {
		//mFrameBuffer = nullptr;
		mVertexBuffer = nullptr;
		mProgram = nullptr;
		mShaderBuffers.clear();
		mTextures.clear();
	}

	// Draw Without Indices
	void DrawArrays(GLenum mode, unsigned int first, unsigned int count);

	// Draw With Indices
	void DrawElements(GLenum mode, unsigned int count, unsigned int offset = 0);
private:
	struct TextureWithIndex {
		TextureWithIndex(unsigned int id, std::shared_ptr<ShaderResource> tex)
			:index(id)
			,texture(tex) {}

		unsigned int index;
		std::shared_ptr<ShaderResource> texture;
	};

	struct BufferWithIndex {
		BufferWithIndex(unsigned int id, std::shared_ptr<ShaderBuffer> bf)
			:index(id)
			,buffer(bf) {}

		unsigned int index;
		std::shared_ptr<ShaderBuffer> buffer;
	};

private:
	GPUDevice* mDevice;

	// ��ǰ��Ⱦ֡��Ҫ�󶨵���Դ
	// Frame Buffer
	// std::shared_ptr<FrameBuffer> mFrameBuffer;
	// Vertex Buffer
	std::shared_ptr<VertexBuffer> mVertexBuffer;
	// Uniform Buffer
	std::vector<BufferWithIndex> mShaderBuffers;
	// GPU Shader Program
	std::shared_ptr<GPUProgram> mProgram;
	// GPU Texture
	std::vector<TextureWithIndex> mTextures;
};