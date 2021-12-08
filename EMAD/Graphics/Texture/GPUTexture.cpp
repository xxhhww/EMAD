#include "GPUTexture.h"

GPUTexture::GPUTexture(const std::string& name, GPUDevice* device)
	:ShaderResource(name, ResourceType::Texture, device){
	glGenTextures(1, &mResourceID);
}

void GPUTexture::Update(GPUSampler::ptr sampler, GPUTexDesc::ptr texDesc)
{
	mSampler = sampler;
	mTexDesc = texDesc;
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
