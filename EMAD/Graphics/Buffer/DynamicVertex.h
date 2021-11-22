#pragma once
#include <vector>
#include <glm/glm.hpp>

// ������������(λ�á���ɫ�����ߡ����������)
enum class AttrType {
	POSITION_3D = 0,
	COLOR_RGB,
	COLOR_RGBA, // ����͸��ͨ��
	NORMAL_3D,
	TEXCOORD_2D,
};

// ����ģ��
template<AttrType T> struct VertexAttrMap;
// ���ݶ�Ӧ�Ķ����������ͣ������ػ�ģ��
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

// ����Ĳ���
class VertexLayout {
public:
	// �����е�Ԫ��
	struct Element {
		Element(AttrType type, size_t index, size_t offset)
			:mAttrType(type)
			,mIndex(index)
			,mOffset(offset) {}

		AttrType mAttrType; // ������������
		size_t mIndex; // �������ڲ����е�����λ��(��˳��)
		size_t mOffset; // �������ڲ����е�ƫ����
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

	// ���AttrType��Ӧ���ֽڳ���
	static size_t GetByteSizeByType(AttrType type);

	// ��Ӳ���Ԫ��
	template<AttrType type>
	void Attach() {
		mElements.emplace_back(type, mIndex, GetByteLength());
		mIndex++;
	}

	// ���ز�����Ԫ�ص����ֽڳ��ȣ�����Ϊ��һ����Ԫ�ص�ƫ����
	size_t GetByteLength() const;

	// ���ݶ����������ͼ������������ض�Ӧ�Ĳ����е�Ԫ��
	// indexΪ�����е�ǰ���͵ľֲ�Index,����һ����������ж����������,��index���Ƕ������������͵�������������ȫ�ֵ�����
	template<AttrType type>
	const Element& Resolve(unsigned int index = 0) const {
		// TODO indexԽ��
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
	// indexΪ���ֵ�ȫ��Index��Resolve�е�index��ͬ
	inline const Element& ResolveByIndex(unsigned int index) const {
		// TODO indexԽ��
		return mElements[index];
	}
	
	inline const std::vector<Element>& GetElements() const {
		return mElements;
	}
private:
	unsigned int mIndex = 0;
	std::vector<Element> mElements;
};

// ���������ӳ��
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
		// TODO��������Ƿ�ƥ��

		//���޸�λ��
		char* pTarget = mData + mLayout.ResolveByIndex(index).mOffset;
		//TODO�ж������Ƿ�һ��
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
	void* mData = nullptr; // ��VertexDataArrayӳ�����������
};


// ��Ŷ������ݵ�����
class VertexDataArray {
public:
	VertexDataArray(const VertexLayout& layout);

	// �ռ������Կ��ܱȽϵ�Ч�������ٸĽ�
	template<typename ...Args>
	void EmplaceBack(Args&& ...args) {
		// �����ڴ�ռ�
		this->Resize(mVertexData.size() + mLayout.GetByteLength());
		this->Back().SetAttrByIndex(0u, std::forward<Args>(args)...);
	}

	// VBO��
	void BindVBO() const;

	// ���������ض�������ӳ��
	Vertex operator[](size_t i);
	// �������һ������ӳ��
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
	// ���ؿ�ӳ�䶥��ĸ���
	inline size_t GetVertexNums() const {
		return mVertexData.size() / mLayout.GetByteLength();
	}
private:
	VertexLayout mLayout; // ��������Ĳ���
	std::vector<char> mVertexData; // ��������Ķ�������
};