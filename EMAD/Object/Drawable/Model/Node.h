#pragma once
#include "../../../EMath.h"

#include <vector>
#include <string>
#include <memory>

class Program;
class Mesh;
class Node {
public:
	Node(const std::string& name, const glm::mat4& baseTrans) 
	:mName(name)
	,mBaseTrans(baseTrans)
	{}

	void draw(std::shared_ptr<Program> program, const glm::mat4& trans) const noexcept;

	void addMeshPtr(std::shared_ptr<Mesh> meshPtr) noexcept{ 
		mMeshPtrs.emplace_back(meshPtr); 
	}
	void addChildPtr(std::unique_ptr<Node> nodePtr) noexcept {
		mChildPtrs.emplace_back(std::move(nodePtr));
	}
private:
	std::string mName;
	glm::mat4 mBaseTrans;
	std::vector<std::shared_ptr<Mesh>> mMeshPtrs;
	std::vector<std::unique_ptr<Node>> mChildPtrs;
};