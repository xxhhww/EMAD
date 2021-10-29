#pragma once
#include "../stb_image.h"

#include <string>
#include <map>
#include <vector>
#include <memory>

class Texture {
public:
	using ptr = std::shared_ptr<Texture>;

	Texture(const unsigned int id, const std::string& tag)
		:mID(id)
		, mTag(tag) {}

	const unsigned int getID() const { return mID; }
	const std::string& getTag() const { return mTag; }
private:
	unsigned int mID;
	std::string mTag; //��ΪΨһ��ʶ��
};

class TextureMgr {
public:
	static Texture::ptr LoadTexture2D(const std::string& dir, const std::string& fileName);
	static Texture::ptr LoadCubeMap(const std::string& dir, std::vector<std::string>& fileNames);
private:
	// ���������Ƿ��Ѿ����ظ�Texture
	static bool IsLoaded(const std::string& path) noexcept;

	using TexMap = std::map<std::string, Texture::ptr>;
	static TexMap& GetMap() noexcept {
		static TexMap sTextures;
		return sTextures;
	}
};