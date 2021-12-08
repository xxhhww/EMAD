#pragma once
#include <Graphics/GPUResource.h>

// 纹理采样信息描述
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

// 纹理基础信息描述
class GPUTexDesc {
public:
	using ptr = std::shared_ptr<GPUTexDesc>;
public:
	GLenum Type; // 纹理类型 2D、3D、cube
	GLuint MipLevel;
	GLuint InternalFormat;
	GLsizei Width;
	GLsizei Height;
	GLenum SourceFormat; //源图的格式
	GLenum DataType; //数据类型(从文件导入的用户资产纹理一般是Byte类型，而渲染目标纹理一般是float类型)

public:
	static GPUTexDesc::ptr Gen2D(GLuint level, GLuint iFormat, GLsizei width, GLsizei height, GLenum sFormat, GLenum dataType);
	static GPUTexDesc::ptr GenCube(GLuint level, GLuint iFormat, GLsizei width, GLsizei height, GLenum sFormat, GLenum dataType);
};



class ShaderResource : public GPUResource {
public:
	using ptr = std::shared_ptr<ShaderResource>;
public:
	ShaderResource(const std::string& name, ResourceType type, GPUDevice* device)
		:GPUResource(name, type, device) {}

	~ShaderResource() {
		glDeleteTextures(1, &mResourceID);
	}

	inline GPUSampler::ptr GetSampler() const {
		return mSampler;
	}

	inline GPUTexDesc::ptr GetTexDesc() const {
		return mTexDesc;
	}

protected:
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
