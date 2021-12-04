#pragma once

#include "GPUResource.h"
#include "../Core/Singleton.h"
#include "../Core/NonCopyable.h"

#include <map>
#include <string>
#include <vector>
#include <memory>

class GPUContext;
class GPUProgram;
class AssetTexture;
class GPUTexture;
class GPUSampler;
class GPUTexDesc;
class VertexBuffer;
class UniformBuffer;
class VertexDataArray;
class FrameBuffer;

// GPU设备，用于创建GPU资源与GPU上下文
class GPUDevice : public Singleton<GPUDevice>, public NonCopyable {
public:
	GPUDevice() {
		mMainContext = std::make_shared<GPUContext>(this);
	}

	inline std::shared_ptr<GPUContext> GetContext() {
		return mMainContext;
	}

	// 检查Device是否已经创建并存放了以name命名的GPUResource
	inline bool HasGPUResource(const std::string& name) {
		return mResources.find(name) == mResources.end() ? false : true;
	}

	// 配合HasGPUResource使用
	template<typename T>
	std::shared_ptr<T> Get(const std::string& name) {
		return std::dynamic_pointer_cast<T>(mResources[name]);
	}

	// Create Texture2D Which is Loaded From Users' file
	std::shared_ptr<AssetTexture> CreateAssetTexture2D(const std::string& fileName, std::shared_ptr<GPUSampler>& sampler, bool isFlip = false);
	// Create CubeTexture Which is Loaded From Users' file
	std::shared_ptr<AssetTexture> CreateAssetCubeTexture(std::vector<std::string>& fileNames, std::shared_ptr<GPUSampler>& sampler, bool isFlip = false);

	// Create Texture Which is used as RenderTarget
	std::shared_ptr<GPUTexture> CreateGPUTexture(const std::string& name, std::shared_ptr<GPUSampler>& sampler, std::shared_ptr<GPUTexDesc>& desc);
	
	// Create Shader Program
	std::shared_ptr<GPUProgram> CreateGPUProgram(const std::string& name);

	// Create Vertex Buffer Without Index
	std::shared_ptr<VertexBuffer> CreateVertexBuffer(const std::string& name, const VertexDataArray& vda);
	// Create Vertex Buffer With Index
	std::shared_ptr<VertexBuffer> CreateVertexBuffer(const std::string& name, const VertexDataArray& vda, const std::vector<unsigned int>& idv);

	std::shared_ptr<UniformBuffer> CreateUniformBuffer(const std::string& name, GLenum usage, size_t bufferSize);

	std::shared_ptr<FrameBuffer> CreateFrameBuffer(const std::string& name);

private:
	std::shared_ptr<GPUContext> mMainContext;
	std::map<std::string, std::shared_ptr<GPUResource>> mResources;
};