#pragma once


#include <memory>

class GPUContext;
class VertexBuffer;

class Quad {
public:
	static void Render(std::shared_ptr<GPUContext> context);
	static std::shared_ptr<VertexBuffer> GetVB();
};