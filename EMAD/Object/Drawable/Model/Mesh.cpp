#include "Mesh.h"
#include "../../../stb_image.h"
#include "../../../Exception.h"

void Mesh::draw(std::shared_ptr<Program> program, const glm::mat4& trans) noexcept
{
	mAccumulatedTrans = trans;
    
    program->activate();
    
    program->setBool("hasSpec", mHasSpec);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mDiffuseTex);
    if (mHasSpec) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mSpecularTex);
    }
    
    Drawable::draw(program);
    
}
