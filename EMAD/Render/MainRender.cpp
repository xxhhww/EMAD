#include "MainRender.h"
#include "Pass/GBufferPass.h"
#include "Pass/LightPass.h"

void RenderService::Init()
{
	GBufferPass::Instance()->Init();
	LightPass::Instance()->Init();
}

void RenderService::Render(std::shared_ptr<RenderContext> rContext)
{
	GBufferPass::Instance()->Render(rContext);
	LightPass::Instance()->Render(rContext);
}
