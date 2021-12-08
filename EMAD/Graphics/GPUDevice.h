#pragma once

#include "GPUResource.h"

#include <Core/Singleton.h>
#include <Core/NonCopyable.h>

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
	GPUDevice();

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

	// 创建新资源
	template<typename T>
	std::shared_ptr<T> Create(const std::string& name) {
		// 资源不存在
		if (mResources.find(name) == mResources.end()) {
			mResources.insert(
				std::pair<std::string, std::shared_ptr<GPUResource>>(name, std::make_shared<T>(name, this))
			);
		}
		return Get<T>(name);
	}

	// 释放资源
	void Release(std::shared_ptr<GPUResource> resource) {
		// 资源存在
		if (mResources.find(resource->GetName()) != mResources.end()) {
			mResources.erase(resource->GetName());
		}
	}

private:
	std::shared_ptr<GPUContext> mMainContext;
	std::map<std::string, std::shared_ptr<GPUResource>> mResources;
};