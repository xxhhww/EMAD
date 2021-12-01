#include "GPUDevice.h"

#include "GPUContext.h"
#include "Texture/AssetTexture.h"
#include "Texture/GPUTexture.h"
#include "Shader/GPUProgram.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/UniformBuffer.h"
#include "Buffer/FrameBuffer.h"

std::shared_ptr<AssetTexture> GPUDevice::CreateAssetTexture2D(const std::string& fileName, std::shared_ptr<GPUSampler>& sampler, bool isFlip)
{
	if (mResources.find(fileName) == mResources.end()) {
		mResources.insert(std::make_pair(fileName, std::make_shared<AssetTexture>(fileName, sampler, isFlip, this)));
	}
	return std::dynamic_pointer_cast<AssetTexture>(mResources[fileName]);
}

std::shared_ptr<AssetTexture> GPUDevice::CreateAssetCubeTexture(std::vector<std::string>& fileNames, std::shared_ptr<GPUSampler>& sampler, bool isFlip)
{
	std::string tempName = AssetTexture::GenResourceName(fileNames);
	if (mResources.find(tempName) == mResources.end()) {
		mResources.insert(std::make_pair(tempName, std::make_shared<AssetTexture>(tempName, sampler, isFlip, this)));
	}
	return std::dynamic_pointer_cast<AssetTexture>(mResources[tempName]);
}

std::shared_ptr<GPUTexture> GPUDevice::CreateGPUTexture(const std::string& name, std::shared_ptr<GPUSampler>& sampler, std::shared_ptr<GPUTexDesc>& desc)
{
	if (mResources.find(name) == mResources.end()) {
		mResources.insert(std::make_pair(name, std::make_shared<GPUTexture>(name, sampler, desc, this)));
	}
	return std::dynamic_pointer_cast<GPUTexture>(mResources[name]);
}

std::shared_ptr<GPUProgram> GPUDevice::CreateGPUProgram(const std::string& name)
{
	if (mResources.find(name) == mResources.end()) {
		mResources.insert(std::make_pair(name, std::make_shared<GPUProgram>(name, this)));
	}
	return std::dynamic_pointer_cast<GPUProgram>(mResources[name]);
}

std::shared_ptr<VertexBuffer> GPUDevice::CreateVertexBuffer(const std::string& name, const VertexDataArray& vda)
{
	if (mResources.find(name) == mResources.end()) {
		mResources.insert(std::make_pair(name, std::make_shared<VertexBuffer>(name, vda, this)));
	}
	return std::dynamic_pointer_cast<VertexBuffer>(mResources[name]);
}

std::shared_ptr<VertexBuffer> GPUDevice::CreateVertexBuffer(const std::string& name, const VertexDataArray& vda, const std::vector<unsigned int>& idv)
{
	if (mResources.find(name) == mResources.end()) {
		mResources.insert(std::make_pair(name, std::make_shared<VertexBuffer>(name, vda, idv, this)));
	}
	return std::dynamic_pointer_cast<VertexBuffer>(mResources[name]);
}

std::shared_ptr<UniformBuffer> GPUDevice::CreateUniformBuffer(const std::string& name, GLenum usage, size_t bufferSize)
{
	if (mResources.find(name) == mResources.end()) {
		mResources.insert(std::make_pair(name, std::make_shared<UniformBuffer>(name, usage, bufferSize, this)));
	}
	return std::dynamic_pointer_cast<UniformBuffer>(mResources[name]);
}


std::shared_ptr<FrameBuffer> GPUDevice::CreateFrameBuffer(const std::string& name)
{
	if (mResources.find(name) == mResources.end()) {
		mResources.insert(std::make_pair(name, std::make_shared<FrameBuffer>(name, this)));
	}
	return std::dynamic_pointer_cast<FrameBuffer>(mResources[name]);
}
