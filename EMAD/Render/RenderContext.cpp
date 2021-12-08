#include "RenderContext.h"
#include "RenderBuffers.h"


RenderContext::RenderContext()
{
	mRenderBuffers = std::make_shared<RenderBuffers>();
	mRenderList = std::make_shared<RenderList>();
}
