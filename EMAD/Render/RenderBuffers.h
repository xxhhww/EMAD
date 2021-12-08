#pragma once

// 不同于OpenGL中的RenderBuffer
// 下文所创建的RenderBuffers是GPUTexture的缓存集合
// 它用来保存各个Pass渲染出来的纹理缓存
// 而这些纹理缓存，又可以被其他Pass所使用

#include <memory>

class GPUTexture;

struct RenderBuffers {
	RenderBuffers();
	~RenderBuffers();

	// 当屏幕宽高与当前场景纹理设置的宽高不同时，需要更新场景纹理的设置
	void Update(unsigned int Width, unsigned int Height);

	struct GBuffer {
		std::shared_ptr<GPUTexture> BaseColor; // Diffuse Color
		std::shared_ptr<GPUTexture> Position; // World Position
		std::shared_ptr<GPUTexture> Normal; // World Normal
		std::shared_ptr<GPUTexture> Other; // Metallic/Roughness/Ao
	} MyGBuffer;
	unsigned int MyWidth; // 场景纹理的宽度
	unsigned int MyHeight; // 场景纹理的高度 
	// Other Buffers
};