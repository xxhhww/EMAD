#pragma once
#include <vector>
#include <glm/glm.hpp>

// 顶点属性类型(位置、颜色、法线、纹理坐标等)
enum class AttrType {
	POSITION_3D = 0,
	COLOR_RGB,
	COLOR_RGBA, // 包含透明通道
	NORMAL_3D,
	TEXCOORD_2D,
};

// 泛化模板
template<AttrType T> struct VertexAttrMap;
// 根据对应的顶点属性类型，构造特化模板
template<> struct VertexAttrMap<AttrType::POSITION_3D> {
	using SysType = glm::vec3;
};
template<> struct VertexAttrMap<AttrType::COLOR_RGB> {
	using SysType = glm::vec3;

};
template<> struct VertexAttrMap<AttrType::COLOR_RGBA> {
	using SysType = glm::vec4;

};
template<> struct VertexAttrMap<AttrType::NORMAL_3D> {
	using SysType = glm::vec3;

};
template<> struct VertexAttrMap<AttrType::TEXCOORD_2D> {
	using SysType = glm::vec2;
};

// 顶点的布局
class VertexLayout {
public:
	// 布局中的元素
	struct Element {
		Element(AttrType type, size_t index, size_t offset)
			:mAttrType(type)
			,mIndex(index)
			,mOffset(offset) {}

		AttrType mAttrType; // 顶点属性类型
		size_t mIndex; // 此属性在布局中的索引位置(按顺序)
		size_t mOffset; // 此属性在布局中的偏移量
	};

public:
	template<template<AttrType> class T, typename ...Args>
	static auto SwitchBridge(AttrType type, Args&& ...args) {
		switch (type)
		{
		case AttrType::POSITION_3D:
			return T<AttrType::POSITION_3D>::Exec(std::forward<Args>(args)...);
		case AttrType::COLOR_RGB:
			return T<AttrType::POSITION_3D>::Exec(std::forward<Args>(args)...);
		case AttrType::COLOR_RGBA:
			return T<AttrType::POSITION_3D>::Exec(std::forward<Args>(args)...);
		case AttrType::NORMAL_3D:
			return T<AttrType::POSITION_3D>::Exec(std::forward<Args>(args)...);
		case AttrType::TEXCOORD_2D:
			return T<AttrType::POSITION_3D>::Exec(std::forward<Args>(args)...);
		}
	}

	// 获得AttrType对应的字节长度
	static size_t GetByteSizeByType(AttrType type);

	// 添加布局元素
	template<AttrType type>
	void Attach() {
		mElements.emplace_back(type, mIndex, GetByteLength());
		mIndex++;
	}

	// 返回布局中元素的总字节长度，可作为下一个新元素的偏移量
	size_t GetByteLength() const;

	// 根据顶点属性类型及其索引，返回对应的布局中的元素
	// index为布局中当前类型的局部Index,比如一个顶点可能有多个纹理坐标,而index就是对纹理坐标类型的索引，它不是全局的索引
	template<AttrType type>
	const Element& Resolve(unsigned int index = 0) const {
		// TODO index越界
		unsigned int tempIndex = 0;
		for (const auto& e : mElements) {
			if (e.mAttrType == type) {
				if (tempIndex == index) {
					return e;
				}
				else {
					tempIndex++;
				}
			}
		}
	}
	// index为布局的全局Index与Resolve中的index不同
	inline const Element& ResolveByIndex(unsigned int index) const {
		// TODO index越界
		return mElements[index];
	}
	
	inline const std::vector<Element>& GetElements() const {
		return mElements;
	}
private:
	unsigned int mIndex = 0;
	std::vector<Element> mElements;
};

// 单个顶点的映射
class Vertex {
friend class VertexDataArray;
public:
	Vertex(const VertexLayout& parent, void* data);

	template<AttrType type>
	auto& Attr(unsigned int index = 0u) {
		char* pTarget = mData + mLayout.Resolve<type>(index).mOffset;
		return *reinterpret_cast<typename VertexAttrMap<type>::SysType*>(pTarget);
	}

private:
	template<AttrType type>
	class AssignableLookUp {
	public:
		template<typename T>
		static bool Exec(T&& val) {
			return std::is_assignable<typename VertexAttrMap<type>::SysType, T>::value;
		}
	};

	template<typename T>
	void SetAttrByIndex(unsigned int index, T val) {
		// TODO检测类型是否匹配

		//待修改位置
		char* pTarget = mData + mLayout.ResolveByIndex(index).mOffset;
		//TODO判断类型是否一致
		//*reinterpret_cast<T*>(pTarget) = val;
		*reinterpret_cast<std::remove_reference<T>::type*>(pTarget) = val;
	}

	template<typename FirstVal, typename ...LeftVals>
	void SetAttrByIndex(unsigned int index, FirstVal&& fVal, LeftVals&& ...lVals) {
		SetAttrByIndex(index, std::forward<FirstVal>(fVal));
		SetAttrByIndex(index + 1, std::forward<LeftVals>(lVals)...);
	}
private:
	const VertexLayout& mLayout;
	void* mData = nullptr; // 从VertexDataArray映射出来的数据
};


// 存放顶点数据的数组
class VertexDataArray {
public:
	VertexDataArray(const VertexLayout& layout);

	// 空间分配策略可能比较低效，后续再改进
	template<typename ...Args>
	void EmplaceBack(Args&& ...args) {
		// 分配内存空间
		this->Resize(mVertexData.size() + mLayout.GetByteLength());
		this->Back().SetAttrByIndex(0u, std::forward<Args>(args)...);
	}

	// VBO绑定
	void BindVBO() const;

	// 按索引返回顶点数据映射
	Vertex operator[](size_t i);
	// 返回最后一个顶点映射
	inline Vertex Back() {
		return Vertex(GetVertexLayout(), mVertexData.data() + mLayout.GetByteLength() * (GetVertexNums() - 1));
	}
	inline void Resize(size_t byteSize) {
		if (mVertexData.size() > byteSize)
			return;
		mVertexData.resize(byteSize);
	}
	inline const VertexLayout& GetVertexLayout() const {
		return mLayout;
	}
	// 返回可映射顶点的个数
	inline size_t GetVertexNums() const {
		return mVertexData.size() / mLayout.GetByteLength();
	}
private:
	VertexLayout mLayout; // 单个顶点的布局
	std::vector<char> mVertexData; // 存放连续的顶点数据
};