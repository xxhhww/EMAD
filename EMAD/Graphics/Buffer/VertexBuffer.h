#pragma once

#include "DynamicVertex.h"

#include <Graphics/GPUResource.h>

// OpenGL�е�VAO
class VertexBuffer : public GPUResource {
public:
	using ptr = std::shared_ptr<VertexBuffer>;
public:
	VertexBuffer(const std::string& name, GPUDevice* device)
		:GPUResource(name, ResourceType::VertexBuffer, device)
		,mIndexSize(0){
		glGenVertexArrays(1, &mResourceID);
	}

	~VertexBuffer() {
		glDeleteVertexArrays(1, &mResourceID);
	}

	// ���VAO
	inline void Update(const VertexDataArray& vda) {
		glBindVertexArray(mResourceID);
		vda.BindVBO();
		glBindVertexArray(0);
	}

	inline void Update(const VertexDataArray& vda, const std::vector<unsigned int> idv) {
		mIndexSize = idv.size();

		glBindVertexArray(mResourceID);
		vda.BindVBO();
		// ��IndexBuffer
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, idv.size() * sizeof(unsigned int), idv.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
	}

	inline unsigned int GetIndexSize() const {
		return mIndexSize;
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
	// ��Ϊ0ʱ��ʹ��DrawElements
	unsigned int mIndexSize = 0;
};