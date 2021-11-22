#pragma once

#include "../GPUResource.h"
#include "DynamicVertex.h"

// OpenGL中的VAO
class VertexBuffer : public GPUResource {
public:
	// 无Index
	VertexBuffer(const std::string& name, const VertexDataArray& vda, GPUDevice* device)
		:GPUResource(name, ResourceType::VertexBuffer, device) 
		,mIndexSize(0){
		glGenVertexArrays(1, &mResourceID);
		glBindVertexArray(mResourceID);
		vda.BindVBO();
		glBindVertexArray(0);
	}
	// 有Index
	VertexBuffer(const std::string& name, const VertexDataArray& vda, const std::vector<unsigned int>& idv, GPUDevice* device)
		:GPUResource(name, ResourceType::VertexBuffer, device) 
		,mIndexSize(idv.size()){
		glGenVertexArrays(1, &mResourceID);
		glBindVertexArray(mResourceID);
		vda.BindVBO();
		// 绑定IndexBuffer
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, idv.size() * sizeof(unsigned int), idv.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
public:
	// [From GPUResource]
	inline virtual void Activate() override {
		glBindVertexArray(mResourceID);
	}
	// [From GPUResource]
	inline virtual void InActivate() override {
		glBindVertexArray(0);
	}
private:
	// 不为0时，使用DrawElements
	unsigned int mIndexSize = 0;
};