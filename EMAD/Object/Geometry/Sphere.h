#pragma once
#include "../../EMath.h"

#include <assert.h>

#include <map>
#include <vector>

class Sphere {
public:
	using TriangleList = std::pair<std::vector<float>, std::vector<unsigned int>>;
	// nstack与slice为细分参数,其中stack为经线上的划分,slice为纬线上的划分
	static TriangleList MakeTesselated(int stack, int slice) {
		assert(stack >= 3);
		assert(slice >= 3);

		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		constexpr float radius = 1.0f;
		unsigned int nRows = stack - 1; // 以纬线横切出的圆的个数
		unsigned int vertexsPerRow = slice + 1; // 被切出来的圆上的顶点数

		for (unsigned int i = 1; i <= nRows; i++) {
			// 获得当前横切圆的半径
			float theta = E_PI * i / stack;
			float tempRadius = radius * sin(theta);

			// 遍历当前横切圆上的所有顶点并存储(起始点存储两次，为了方便做索引)
			for (unsigned int j = 0; j < vertexsPerRow; j++) {
				float phi = E_2PI * j / slice;

				glm::vec3 pos;
				pos.x = tempRadius * cos(phi);
				pos.y = radius * cos(theta);
				pos.z = tempRadius * sin(phi);

				// 插入数据至容器
				vertices.emplace_back(pos.x);
				vertices.emplace_back(pos.y);
				vertices.emplace_back(pos.z);
			}
		}
		// 添加南北两个极点
		glm::vec3 north(0.0f, radius, 0.0f);
		glm::vec3 south(0.0f, -radius, 0.0f);
		vertices.emplace_back(north.x);
		vertices.emplace_back(north.y);
		vertices.emplace_back(north.z);
		vertices.emplace_back(south.x);
		vertices.emplace_back(south.y);
		vertices.emplace_back(south.z);

		// 球的索引情况
		int size = vertexsPerRow * nRows;
		int tmp = 0;
		int start1 = 0;
		int start2 = vertices.size() - vertexsPerRow - 2;
		int top = size;
		int bottom = size + 1;
		for (int i = 0; i < slice; ++i) {
			indices.push_back(top);
			indices.push_back(start1 + i + 1);
			indices.push_back(start1 + i);
		}

		for (int i = 0; i < slice; ++i) {
			indices.push_back(bottom);
			indices.push_back(start2 + i);
			indices.push_back(start2 + i + 1);
		}

		for (int i = 0; i < nRows - 1; ++i) {
			for (int j = 0; j < slice; ++j) {
				indices.push_back(i * vertexsPerRow + j);
				indices.push_back((i + 1) * vertexsPerRow + j + 1);
				indices.push_back((i + 1) * vertexsPerRow + j);
				indices.push_back(i * vertexsPerRow + j);
				indices.push_back(i * vertexsPerRow + j + 1);
				indices.push_back((i + 1) * vertexsPerRow + j + 1);
			}
		}
		return { std::move(vertices),std::move(indices) };
	}

	static TriangleList Make() {
		return MakeTesselated(12, 24);
	}
};