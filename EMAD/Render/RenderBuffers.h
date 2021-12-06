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

	struct GBuffer {
		std::shared_ptr<GPUTexture> BaseColor; // Diffuse Color
		std::shared_ptr<GPUTexture> Position; // World Position
		std::shared_ptr<GPUTexture> Normal; // World Normal
		std::shared_ptr<GPUTexture> Other; // Metallic/Roughness/Ao
	} MyGBuffer;

	// Other Buffers
};