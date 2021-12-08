#pragma once

// ��ͬ��OpenGL�е�RenderBuffer
// ������������RenderBuffers��GPUTexture�Ļ��漯��
// �������������Pass��Ⱦ������������
// ����Щ�����棬�ֿ��Ա�����Pass��ʹ��

#include <memory>

class GPUTexture;

struct RenderBuffers {
	RenderBuffers();
	~RenderBuffers();

	// ����Ļ����뵱ǰ�����������õĿ�߲�ͬʱ����Ҫ���³������������
	void Update(unsigned int Width, unsigned int Height);

	struct GBuffer {
		std::shared_ptr<GPUTexture> BaseColor; // Diffuse Color
		std::shared_ptr<GPUTexture> Position; // World Position
		std::shared_ptr<GPUTexture> Normal; // World Normal
		std::shared_ptr<GPUTexture> Other; // Metallic/Roughness/Ao
	} MyGBuffer;
	unsigned int MyWidth; // ��������Ŀ��
	unsigned int MyHeight; // ��������ĸ߶� 
	// Other Buffers
};