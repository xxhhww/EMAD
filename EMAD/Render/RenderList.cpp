#include "RenderList.h"
#include "RenderContext.h"
#include "../Graphics/GPUContext.h"
#include "../Graphics/Material/Material.h"

void RenderList::SortDrawCall(DrawCallPass pass)
{
}

void RenderList::ExecDrawCall(std::shared_ptr<RenderContext> renderContext, DrawCallPass pass)
{
	std::vector<DrawCall::ptr>& AllDrawCalls = mDrawCalls[pass];

	for (auto& i : AllDrawCalls) {
		// Bind Material Data(Program¡¢UB¡¢SR)
		Material::BindInfo tempBindInfo;
		tempBindInfo.MyRenderContext = renderContext;
		tempBindInfo.MyDrawCall = i;
		i->MyMaterial->Bind(tempBindInfo);

		// Bind VB
		renderContext->mGPUContext->BindVAO(i->MyGeometryData.VertexBufferPtr);
		// Draw Elements
		renderContext->mGPUContext->DrawElements(
			i->MyGeometryData.Mode,
			i->MyGeometryData.IndexSize,
			0);
	}
}
