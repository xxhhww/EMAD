#pragma once

#include "GPUResource.h"
#include <map>
#include <string>
#include <vector>
#include <memory>

class GPUContext;
class GPUProgram;
class GPUTexture;
class GPUSampler;
class GPUTexDesc;
class VertexBuffer;
class UniformBuffer;
class VertexDataArray;
class FrameBuffer;

// GPU设备，用于创建GPU资源与GPU上下文
class GPUDevice {
public:
	GPUDevice();

	inline std::shared_ptr<GPUContext> GetContext() {
		return mMainContext;
	}

	std::shared_ptr<GPUTexture> CreateGPUTexture(const std::string& name, std::shared_ptr<GPUSampler>& sampler, std::shared_ptr<GPUTexDesc>& desc);
	std::shared_ptr<GPUProgram> CreateGPUProgram(const std::string& name);
	std::shared_ptr<VertexBuffer> CreateVertexBuffer(const std::string& name, const VertexDataArray& vda);
	std::shared_ptr<VertexBuffer> CreateVertexBuffer(const std::string& name, const VertexDataArray& vda, const std::vector<unsigned int>& idv);
	std::shared_ptr<UniformBuffer> CreateUniformBuffer(const std::string& name, GLenum usage, size_t bufferSize);
	std::shared_ptr<FrameBuffer> CreateFrameBuffer(const std::string& name);
	
	
	inline static GPUDevice* Instance() {
		static GPUDevice sDevice;
		return &sDevice;
	}

private:
	std::shared_ptr<GPUContext> mMainContext;
	std::map<std::string, std::shared_ptr<GPUResource>> mResources;
};