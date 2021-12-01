#include "RenderContext.h"
#include "RenderBuffers.h"
#include "RenderList.h"


RenderContext::RenderContext()
{
	mRenderBuffers = std::make_shared<RenderBuffers>();
	mRenderList = std::make_shared<RenderList>();
}

void RenderContext::AddDrawCall(DrawCallPass pass, DrawCall::ptr drawCall)
{
	// TODO ÅÐ¶ÏpassÊÇ·ñ´æÔÚ
	mRenderList->AddDrawCall(pass, drawCall);
}

void RenderContext::ExecDrawCall(DrawCallPass pass)
{
	mRenderList->ExecDrawCall(shared_from_this(), pass);
}
