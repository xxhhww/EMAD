#pragma once

#include <Actor/Actor.h>

#include <Graphics/Buffer/VertexBuffer.h>
#include <Graphics/Material/Material.h>

class Ball : public Actor {
public:
	using ptr = std::shared_ptr<Ball>;
public:
	// Submit DrawCall
	virtual void Submit(std::shared_ptr<RenderContext> renderContext) override;

	virtual void GenCtrlGui() override;

	inline void SetMaterial(Material::ptr material) {
		mMaterial = material;
	}

private:
	Material::ptr mMaterial;
};