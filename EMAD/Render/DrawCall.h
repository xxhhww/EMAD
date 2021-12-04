#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>

class VertexBuffer;
class Material;

// 标记当前DrawCall所处的Pass
// Pass标志着DrawCall的渲染目标不同
// 例如：
// 处于DepthPass中的DrawCall，其任务是将Object的深度信息渲染到对应的RenderTarget(GPUTexture)中
// 处于GBufferPass中的DrawCall，其任务是将Object的几何、材质等信息渲染到GBuffer的GPUTextures中
// 处于ForwardPass中的DrawCall，其任务是将Object直接渲染到最终的SceneQuad上

enum class DrawCallPass {
	Depth = 0,
	GBuffer = 1,
	Forward = 2,
};

// 一个绘制调用，需要的数据
struct DrawCall {
	using ptr = std::shared_ptr<DrawCall>;

	// 需要绘制的物体的几何数据
	struct GeometryData {
		// VertexBuffer中包含了顶点属性数据和顶点索引数据
		std::shared_ptr<VertexBuffer> VertexBufferPtr;
		// 顶点索引的开始绘制位置
		unsigned int StartIndex;
		// 索引个数
		unsigned int IndexSize;
		// 图元拓扑
		GLenum Mode;
	} MyGeometryData;

	// 需要绘制的物体的材质信息
	std::shared_ptr<Material> MyMaterial;

	// 需要绘制的物体的模型变换矩阵
	glm::mat4 MyModelTrans;

	// 需要绘制的物体的世界坐标
	glm::vec3 MyWorldPosition;

	// 需要绘制的物体的缩放情况
	glm::mat4 MyScaling;
};