#pragma once
#include "../Drawable.hpp"

class Mesh : public Drawable{
public:
	Mesh(unsigned int VAO, unsigned int indexSize) {
		mVAO = VAO;
		mIndexSize = indexSize;
		mAccumulatedTrans = glm::mat4{ 1.0f };
	}

	void draw(std::shared_ptr<Program> program, const glm::mat4& trans) noexcept;
	virtual glm::mat4 genModelTrans() const noexcept { return mAccumulatedTrans; }
private:
	glm::mat4 mAccumulatedTrans;
};