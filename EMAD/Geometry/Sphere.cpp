#include "Sphere.h"
#include "../Graphics/GPUDevice.h"
#include "../Graphics/GPUContext.h"
#include "../Graphics/Buffer/DynamicVertex.h"
#include "../Graphics/Buffer/VertexBuffer.h"

#include <sstream>

constexpr float E_PI = 3.14159265f;
constexpr float E_2PI = 6.283185482f;

std::shared_ptr<VertexBuffer> Sphere::GetVB()
{
	// 默认划分
	return GetVB(18, 36);
}

// 详情见 http://www.songho.ca/opengl/gl_sphere.html
// stack表示球沿着经线划分的个数
// slice表示球沿着纬线划分的个数
std::shared_ptr<VertexBuffer> Sphere::GetVB(int stack, int slice)
{
	assert(stack >= 3);
	assert(slice >= 3);
	std::stringstream ss;
	ss << "VB_Sphere" << stack << "/" << slice;
	bool hasCreated = GPUDevice::Instance()->HasGPUResource(ss.str());
	if (hasCreated) {
		return GPUDevice::Instance()->Get<VertexBuffer>(ss.str());
	}

	VertexLayout SphereLayout;
	SphereLayout.Attach<AttrType::POSITION_3D>();
	SphereLayout.Attach<AttrType::TEXCOORD_2D>();
	SphereLayout.Attach<AttrType::NORMAL_3D>();

	VertexDataArray SphereVertexArray{ SphereLayout };

	// Gen Sphere With UVSphere
	constexpr float radius = 1.0f;

	for (unsigned int i = 0; i <= stack; ++i) {
		// 获得当前经线与起始经线的夹角
		float theta = E_PI * i / stack;

		// 遍历当前经线上的切分点
		for (unsigned int j = 0; j <= slice; ++j) {
			// 获得当前切分点与y轴的夹角
			float phi = E_2PI * j / slice;

			// 计算当前切分点的位置
			glm::vec3 pos;
			pos.x = radius * sin(theta) * cos(phi);
			pos.y = radius * sin(theta) * sin(phi);
			pos.z = cos(theta);

			// 插入数据至容器
			SphereVertexArray.EmplaceBack(pos, glm::vec2{ i / stack, j / slice }, pos);
		}
	}

	std::vector<unsigned int> indices;
	unsigned int k1, k2;
	for (int i = 0; i < stack; ++i)
	{
		k1 = i * (slice + 1);     // beginning of current stack
		k2 = k1 + slice + 1;      // beginning of next stack

		for (int j = 0; j < slice; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stack - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	return GPUDevice::Instance()->CreateVertexBuffer(ss.str(), SphereVertexArray, indices);
}
