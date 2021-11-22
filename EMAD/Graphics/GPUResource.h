#pragma once
#include <glad/glad.h>

#include <string>
#include <memory>

class GPUDevice;

// GPU��ʹ�õ���Դ�Ļ�����
class GPUResource {
public:
	// GPU��Դ����
	// 2D����������������������ɫ������ɫ������VAO��֡���桢renderBuffer��uniformBuffer
	enum class ResourceType {
		Texture,
		Shader,
		Program,
		VertexBuffer,
		UniformBuffer,
		FrameBuffer,
		RenderBuffer,
		Other
	};

	GPUResource(const std::string& name, ResourceType type, GPUDevice* device)
		:mName(name)
		,mResourceType(type)
		,mDevice(device){}

	// ����GPUResource����
	virtual void Activate() {}
	// ʧ��GPUResource����
	virtual void InActivate() {}

	inline GLuint GetResourceID() const {
		return mResourceID; 
	}

	inline const std::string& GetName() const { 
		return mName; 
	}
	inline ResourceType GetResourceType() const { 
		return mResourceType; 
	}

	inline const GPUDevice* GetDevice() const { 
		return mDevice; 
	}

protected:
	GLuint mResourceID = 0;
	std::string mName = "";
	ResourceType mResourceType = ResourceType::Other;
	GPUDevice* mDevice = nullptr;
};