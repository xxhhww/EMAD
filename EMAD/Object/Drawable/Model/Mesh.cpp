#include "Mesh.h"

void Mesh::draw(std::shared_ptr<Program> program, const glm::mat4& trans) noexcept
{
	mAccumulatedTrans = trans;
	Drawable::draw(program);
}
