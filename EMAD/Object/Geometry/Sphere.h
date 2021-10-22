#pragma once
#include "../../EMath.h"

#include <assert.h>

#include <map>
#include <vector>

class Sphere {
public:
	using TriangleList = std::pair<std::vector<float>, std::vector<unsigned int>>;
	// nstack��sliceΪϸ�ֲ���,����stackΪ�����ϵĻ���,sliceΪγ���ϵĻ���
	static TriangleList MakeTesselated(int stack, int slice) {
		assert(stack >= 3);
		assert(slice >= 3);

		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		constexpr float radius = 1.0f;
		unsigned int nRows = stack - 1; // ��γ�ߺ��г���Բ�ĸ���
		unsigned int vertexsPerRow = slice + 1; // ���г�����Բ�ϵĶ�����

		for (unsigned int i = 1; i <= nRows; i++) {
			// ��õ�ǰ����Բ�İ뾶
			float theta = E_PI * i / stack;
			float tempRadius = radius * sin(theta);

			// ������ǰ����Բ�ϵ����ж��㲢�洢(��ʼ��洢���Σ�Ϊ�˷���������)
			for (unsigned int j = 0; j < vertexsPerRow; j++) {
				float phi = E_2PI * j / slice;

				glm::vec3 pos;
				pos.x = tempRadius * cos(phi);
				pos.y = radius * cos(theta);
				pos.z = tempRadius * sin(phi);

				// ��������������
				vertices.emplace_back(pos.x);
				vertices.emplace_back(pos.y);
				vertices.emplace_back(pos.z);
			}
		}
		// ����ϱ���������
		glm::vec3 north(0.0f, radius, 0.0f);
		glm::vec3 south(0.0f, -radius, 0.0f);
		vertices.emplace_back(north.x);
		vertices.emplace_back(north.y);
		vertices.emplace_back(north.z);
		vertices.emplace_back(south.x);
		vertices.emplace_back(south.y);
		vertices.emplace_back(south.z);

		// ����������
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