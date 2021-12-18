#pragma once

#include <Core/Singleton.h>
#include <Core/NonCopyable.h>

#include <memory>
#include <vector>

class Actor;
class Camera;
class RenderContext;

// ����������
class Scene : public Singleton<Scene>, public NonCopyable {
public:
	using ptr = std::shared_ptr<Scene>;
public:
	void Init();
	// ���Ƴ���
	void Render(std::shared_ptr<Camera> camera);
	// ���ɳ������ƽ���
	void GenCtrlGui();
private:
	// ���Actor
	// ɾ��Actor
private:
	// ��������
	std::vector<std::shared_ptr<Actor>> mActors;
};