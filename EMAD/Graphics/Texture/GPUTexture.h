#pragma once
#include "../GPUResource.h"

// ���������Ϣ����
class GPUSampler {
public:
	using ptr = std::shared_ptr<GPUSampler>;

public:
	GLint WRAPS;
	GLint WRAPT;
	GLint WRAPR;
	GLint MinFilter;
	GLint MagFilter;

public:
	static GPUSampler::ptr Gen2D(GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter);
	static GPUSampler::ptr GenCube(GLint wrapS, GLint wrapT, GLint wrapR, GLint minFilter, GLint magFilter);
};

// ���������Ϣ����
class GPUTexDesc {
public:
	using ptr = std::shared_ptr<GPUTexDesc>;
public:
	GLenum Type; // �������� 2D��3D��cube
	GLuint MipLevel;
	GLuint InternalFormat;
	GLsizei Width;
	GLsizei Height;
	GLenum SourceFormat; //Դͼ�ĸ�ʽ
	GLenum DataType; //��������(���ļ�������û��ʲ�����һ����Byte���ͣ�����ȾĿ������һ����float����)

public:
	static GPUTexDesc::ptr Gen2D(GLuint level, GLuint iFormat, GLsizei width, GLsizei height, GLenum sFormat, GLenum dataType);
	static GPUTexDesc::ptr GenCube(GLuint level, GLuint iFormat, GLsizei width, GLsizei height, GLenum sFormat, GLenum dataType);
};

class GPUTexture : public GPUResource {
public:
	using ptr = std::shared_ptr<GPUTexture>;

public:
	// ������ȾĿ������dataһ�㶼��nullptr�������û��ʲ�����dataһ���ǽ������л�֮�������
	GPUTexture(const std::string& name, GPUSampler::ptr sampler, GPUTexDesc::ptr texDesc, GPUDevice* device, void* data = nullptr);

	inline GPUSampler::ptr GetSampler() const {
		return mSampler;
	}

	inline GPUTexDesc::ptr GetTexDesc() const {
		return mTexDesc;
	}

private:
	GPUSampler::ptr mSampler;
	GPUTexDesc::ptr mTexDesc;

public:
	// [From GPUResource]
	inline virtual void Activate() override {
		glBindTexture(mTexDesc->Type, mResourceID);
	}
	// [From GPUResource]
	inline virtual void InActivate() override {
		glBindTexture(mTexDesc->Type, 0);
	}
};