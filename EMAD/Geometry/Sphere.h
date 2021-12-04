#pragma once

#include <memory>

class GPUContext;
class VertexBuffer;

class Sphere {
public:
	static std::shared_ptr<VertexBuffer> GetVB();
	// stack与slice为细分参数,其中stack为经线上的划分,slice为纬线上的划分
	static std::shared_ptr<VertexBuffer> GetVB(unsigned int stack, unsigned int slice);
};