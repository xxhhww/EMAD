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

// GPU�豸�����ڴ���GPU��Դ��GPU������
class GPUDevice : public Singleton<GPUDevice>, public NonCopyable {
public:
	GPUDevice();

	inline std::shared_ptr<GPUContext> GetContext() {
		return mMainContext;
	}

	// ���Device�Ƿ��Ѿ��������������name������GPUResource
	inline bool HasGPUResource(const std::string& name) {
		return mResources.find(name) == mResources.end() ? false : true;
	}

	// ���HasGPUResourceʹ��
	template<typename T>
	std::shared_ptr<T> Get(const std::string& name) {
		return std::dynamic_pointer_cast<T>(mResources[name]);
	}

	// ��������Դ
	template<typename T>
	std::shared_ptr<T> Create(const std::string& name) {
		// ��Դ������
		if (mResources.find(name) == mResources.end()) {
			mResources.insert(
				std::pair<std::string, std::shared_ptr<GPUResource>>(name, std::make_shared<T>(name, this))
			);
		}
		return Get<T>(name);
	}

	// �ͷ���Դ
	void Release(std::shared_ptr<GPUResource> resource) {
		// ��Դ����
		if (mResources.find(resource->GetName()) != mResources.end()) {
			mResources.erase(resource->GetName());
		}
	}

private:
	std::shared_ptr<GPUContext> mMainContext;
	std::map<std::string, std::shared_ptr<GPUResource>> mResources;
};