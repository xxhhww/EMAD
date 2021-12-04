#pragma once

#include <memory>

class GPUContext;
class VertexBuffer;

class Cube {
public:
	static std::shared_ptr<VertexBuffer> GetVB();
};