#pragma once
#include "../Drawable.hpp"
#include <string>

class Texture {
public:
	static unsigned int Load(const std::string& fileName, const std::string& directory);
private:
};

class Mesh : public Drawable{
public:
	Mesh(unsigned int VAO, unsigned int indexSize) {
		mVAO = VAO;
		mIndexSize = indexSize;
		mAccumulatedTrans = glm::mat4{ 1.0f };
		mHasSpec = false;
	}

	void setDiffuseTex(unsigned int tex, const std::string& dir) noexcept { mDiffuseTex = tex; mDiffDir = dir; }
	void setSpecularTex(unsigned int tex) noexcept { mSpecularTex = tex; mHasSpec = true; }

	void draw(std::shared_ptr<Program> program, const glm::mat4& trans) noexcept;
	virtual glm::mat4 genModelTrans() const noexcept { return mAccumulatedTrans; }
private:
	glm::mat4 mAccumulatedTrans;

	unsigned int mDiffuseTex;
	unsigned int mSpecularTex;
	std::string mDiffDir;
	bool mHasSpec;
};