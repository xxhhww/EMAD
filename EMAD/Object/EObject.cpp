#include "EObject.h"

EObject::~EObject()
{
	glDeleteVertexArrays(1, &mVAO);
}
