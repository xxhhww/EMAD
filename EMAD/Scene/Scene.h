#pragma once

#include <Core/Singleton.h>
#include <Core/NonCopyable.h>

#include <memory>
#include <vector>

class Actor;
class Camera;
class RenderContext;

// 场景管理器
class Scene : public Singleton<Scene>, public NonCopyable {
public:
	using ptr = std::shared_ptr<Scene>;
public:
	void Init();
	// 绘制场景
	void Render(std::shared_ptr<Camera> camera);
	// 生成场景控制界面
	void GenCtrlGui();
private:
	// 添加Actor
	// 删除Actor
private:
	// 场景物体
	std::vector<std::shared_ptr<Actor>> mActors;
};