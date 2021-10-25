#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Node.h"

class Mesh;
class Program;
struct aiMesh;
struct aiNode;
struct aiScene;

class Model{
public:
	Model(const std::string& name);

	void draw(std::shared_ptr<Program> program) noexcept;
private:
	std::unique_ptr<Node> processNode(aiNode* node) noexcept;
	std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene) noexcept;
private:
	// ģ�͵���������
	std::vector<std::shared_ptr<Mesh>> mMeshPtrs;
	// ģ�͸��ڵ�
	std::unique_ptr<Node> mRootNodePtr;
	// ģ��Ŀ¼
	std::string mDir;
};