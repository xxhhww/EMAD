#include "Drawable.hpp"

class TestQuad : public Drawable {
public:
	TestQuad(const std::string& name = "Quad");
	void setTexture(unsigned int tex) noexcept;
	virtual void draw(std::shared_ptr<Program> program) noexcept override;
private:
	unsigned int mTexture;
};