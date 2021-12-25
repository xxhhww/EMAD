#pragma once

#include <memory>
#include <string>

#include <glad/glad.h>

enum class ShaderType : uint8_t{
	VS = 0x00,
	GS = 0x01,
	PS = 0x02,
	Other = 0x03
};

// GPUShader资源生命周期由GPUProgram管理
class GPUShader{
public:
	using ptr = std::shared_ptr<GPUShader>;
public:

	GPUShader(ShaderType sType, const std::string& shaderCode);
	GPUShader(ShaderType sType, const std::string& dir, const std::string& sName);
	~GPUShader() {
		glDeleteShader(mResourceID);
	}

	inline unsigned int GetResourceID() const {
		return mResourceID;
	}

	inline ShaderType GetShaderType() const {
		return mShaderType;
	}

private:
	unsigned int mResourceID;
	ShaderType mShaderType;
};