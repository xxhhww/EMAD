#include "ShaderResource.h"

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