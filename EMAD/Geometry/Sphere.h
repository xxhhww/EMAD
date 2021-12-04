#pragma once

#include <memory>

class GPUContext;
class VertexBuffer;

class Sphere {
public:
	static std::shared_ptr<VertexBuffer> GetVB();
	// stack��sliceΪϸ�ֲ���,����stackΪ�����ϵĻ���,sliceΪγ���ϵĻ���
	static std::shared_ptr<VertexBuffer> GetVB(unsigned int stack, unsigned int slice);
};