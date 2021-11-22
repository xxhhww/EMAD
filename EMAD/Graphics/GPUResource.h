#pragma once
#include <glad/glad.h>

#include <string>
#include <memory>

class GPUDevice;

// GPU上使用的资源的基础类
class GPUResource {
public:
	// GPU资源类型
	// 2D纹理、立方体纹理、体纹理、着色器、着色器程序、VAO、帧缓存、renderBuffer、uniformBuffer
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

	// 激活GPUResource对象
	virtual void Activate() {}
	// 失活GPUResource对象
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