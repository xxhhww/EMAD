#include "Ball.h"
#include <Render/DrawCall.h>
#include <Render/RenderContext.h>
#include <Geometry/Sphere.h>


void Ball::Submit(std::shared_ptr<RenderContext> renderContext)
{
	DrawCall::ptr tempDrawCall = std::make_shared<DrawCall>();

	VertexBuffer::ptr SphereVB = Sphere::GetVB();
	tempDrawCall->MyGeometryData.VertexBufferPtr = SphereVB;
	tempDrawCall->MyGeometryData.IndexSize = SphereVB->GetIndexSize();
	tempDrawCall->MyGeometryData.StartIndex = 0u;
	tempDrawCall->MyGeometryData.Mode = GL_TRIANGLES;

	tempDrawCall->MyScaling = mScaling;
	tempDrawCall->MyWorldPosition = mPosition;
	tempDrawCall->MyModelTrans = glm::identity<glm::mat4>();

	tempDrawCall->MyMaterial = mMaterial;

	renderContext->AddDrawCall(DrawCallPass::GBuffer, tempDrawCall);
}

void Ball::GenCtrlGui(){

	mMaterial->GenCtrlGui();
}
