#include "GPUTexture.h"

GPUSampler::ptr GPUSampler::Gen2D(GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter)
{
	GPUSampler::ptr temp = std::make_shared<GPUSampler>();
	temp->WRAPS = wrapS;
	temp->WRAPT = wrapT;
	temp->MinFilter = minFilter;
	temp->MagFilter = magFilter;

	return temp;
}

GPUSampler::ptr GPUSampler::GenCube(GLint wrapS, GLint wrapT, GLint wrapR, GLint minFilter, GLint magFilter)
{
	GPUSampler::ptr temp = std::make_shared<GPUSampler>();
	temp->WRAPS = wrapS;
	temp->WRAPT = wrapT;
	temp->WRAPR = wrapR;
	temp->MinFilter = minFilter;
	temp->MagFilter = magFilter;

	return temp;
}

GPUTexDesc::ptr GPUTexDesc::Gen2D(GLuint level, GLuint iFormat, GLsizei width, GLsizei height, GLenum sFormat, GLenum dataType)
{
	GPUTexDesc::ptr temp = std::make_shared<GPUTexDesc>();
	temp->Type = GL_TEXTURE_2D;
	temp->MipLevel = level;
	temp->InternalFormat = iFormat;
	temp->Width = width;
	temp->Height = height;
	temp->SourceFormat = sFormat;
	temp->DataType = dataType;

	return temp;
}

GPUTexDesc::ptr GPUTexDesc::GenCube(GLuint level, GLuint iFormat, GLsizei width, GLsizei height, GLenum sFormat, GLenum dataType)
{
	GPUTexDesc::ptr temp = std::make_shared<GPUTexDesc>();
	temp->Type = GL_TEXTURE_CUBE_MAP;
	temp->MipLevel = level;
	temp->InternalFormat = iFormat;
	temp->Width = width;
	temp->Height = height;
	temp->SourceFormat = sFormat;
	temp->DataType = dataType;

	return temp;
}

GPUTexture::GPUTexture(const std::string& name, GPUSampler::ptr sampler, GPUTexDesc::ptr texDesc, GPUDevice* device)
	:GPUResource(name, ResourceType::Texture, device)
	,mSampler(sampler)
	,mTexDesc(texDesc){
	
	glGenTextures(1, &mResourceID);
	switch (texDesc->Type)
	{
	case GL_TEXTURE_2D:
		glBindTexture(GL_TEXTURE_2D, mResourceID);
		glTexImage2D(GL_TEXTURE_2D,
				mTexDesc->MipLevel, 
				mTexDesc->InternalFormat, 
				mTexDesc->Width, 
				mTexDesc->Height, 
				0, 
				mTexDesc->SourceFormat, 
				mTexDesc->DataType,
				nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mSampler->WRAPS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mSampler->WRAPT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mSampler->MinFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mSampler->MagFilter);
		glBindTexture(GL_TEXTURE_2D, 0);
		break;
	case GL_TEXTURE_3D:
	case GL_TEXTURE_CUBE_MAP:
		glBindTexture(GL_TEXTURE_CUBE_MAP, mResourceID);
		for (size_t i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				mTexDesc->MipLevel,
				mTexDesc->InternalFormat,
				mTexDesc->Width,
				mTexDesc->Height,
				0,
				mTexDesc->SourceFormat,
				mTexDesc->DataType,
				nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mSampler->WRAPS);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mSampler->WRAPT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, mSampler->WRAPR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mSampler->MinFilter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mSampler->MagFilter);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		break;
	default:
		break;
	}
}
