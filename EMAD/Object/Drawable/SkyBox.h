#pragma once

#include "Drawable.hpp"
#include <vector>
#include <memory>

class Texture;
class SkyBox : public Drawable {
public:
	SkyBox(const std::string& name, const std::string& dir, std::vector<std::string>& fileNames);

	virtual void draw(std::shared_ptr<Program> program) noexcept override;
private:
	std::shared_ptr<Texture> mCubeMapPtr;
};