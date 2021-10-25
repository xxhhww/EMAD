#include "Node.h"
#include "Mesh.h"

void Node::draw(std::shared_ptr<Program> program, const glm::mat4& trans) const noexcept
{
	const glm::mat4 accumulatedTrans = trans * mBaseTrans;
	for (const auto& i : mMeshPtrs) {
		i->draw(program, accumulatedTrans);
	}
	for (const auto& i : mChildPtrs) {
		i->draw(program, accumulatedTrans);
	}
}
