#include "DynamicVertex.h"
#include "../GPUResource.h"

template<AttrType type>
struct TypeSizeLookUp {
	static size_t Exec() {
		return sizeof(typename VertexAttrMap<type>::SysType);
	}
};

size_t VertexLayout::GetByteSizeByType(AttrType type)
{
	return SwitchBridge<TypeSizeLookUp>(type);
}

size_t VertexLayout::GetByteLength() const
{
	if (mElements.empty()) {
		return static_cast<size_t>(0);
	}
	const auto& i = mElements.back();
	return (i.mOffset + GetByteSizeByType(i.mAttrType));
}

Vertex::Vertex(const VertexLayout& layout, void* data)
	:mLayout(layout)
	,mData(data){}

VertexDataArray::VertexDataArray(const VertexLayout& layout)
	:mLayout(std::move(const_cast<VertexLayout&>(layout))){}

void VertexDataArray::BindVBO() const
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mVertexData.size(), mVertexData.data(), GL_STATIC_DRAW);
	// 绑定各个顶点属性
	const std::vector<VertexLayout::Element>& temp = mLayout.GetElements();
	for (size_t i = 0; i < temp.size(); i++) {
		glVertexAttribPointer(
			i,
			VertexLayout::GetByteSizeByType(temp[i].mAttrType) / sizeof(glm::vec1),
			GL_FLOAT,
			GL_FALSE,
			mLayout.GetByteLength(),
			(void*)temp[i].mOffset
		);
		glEnableVertexAttribArray(i);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Vertex VertexDataArray::operator[](size_t i)
{
	// TODO 越界了,报异常
	if (i + 1 > GetVertexNums()) {

	}
	return Vertex(mLayout, mVertexData.data() + mLayout.GetByteLength() * i);
}
