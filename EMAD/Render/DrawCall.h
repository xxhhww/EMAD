#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>

class VertexBuffer;
class Material;

// ��ǵ�ǰDrawCall������Pass
// Pass��־��DrawCall����ȾĿ�겻ͬ
// ���磺
// ����DepthPass�е�DrawCall���������ǽ�Object�������Ϣ��Ⱦ����Ӧ��RenderTarget(GPUTexture)��
// ����GBufferPass�е�DrawCall���������ǽ�Object�ļ��Ρ����ʵ���Ϣ��Ⱦ��GBuffer��GPUTextures��
// ����ForwardPass�е�DrawCall���������ǽ�Objectֱ����Ⱦ�����յ�SceneQuad��

enum class DrawCallPass {
	Depth = 0,
	GBuffer = 1,
	Forward = 2,
};

// һ�����Ƶ��ã���Ҫ������
struct DrawCall {
	using ptr = std::shared_ptr<DrawCall>;

	// ��Ҫ���Ƶ�����ļ�������
	struct GeometryData {
		// VertexBuffer�а����˶����������ݺͶ�����������
		std::shared_ptr<VertexBuffer> VertexBufferPtr;
		// ���������Ŀ�ʼ����λ��
		unsigned int StartIndex;
		// ��������
		unsigned int IndexSize;
		// ͼԪ����
		GLenum Mode;
	} MyGeometryData;

	// ��Ҫ���Ƶ�����Ĳ�����Ϣ
	std::shared_ptr<Material> MyMaterial;

	// ��Ҫ���Ƶ������ģ�ͱ任����
	glm::mat4 MyModelTrans;

	// ��Ҫ���Ƶ��������������
	glm::vec3 MyWorldPosition;

	// ��Ҫ���Ƶ�������������
	glm::mat4 MyScaling;
};