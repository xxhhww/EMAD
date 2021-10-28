#include "Renderer.h"

#include "Shader/ShaderProgram.h"

#include "Object/Camera/Camera.h"
#include "Object/Light/PointLight.h"
#include "Object/Light/DirectLight.h"

#include "Object/Drawable/TestCube.h"
#include "Object/Drawable/TestCube2.h"
#include "Object/Drawable/TestPlane.h"
#include "Object/Drawable/Model/Model.h"

#include "stb_image.h"
#include "Exception.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

App::App(int width, int height, const std::string& name) noexcept
	:mWindow(width, height, name){
    // 启东深度缓冲测试
    glEnable(GL_DEPTH_TEST);

    mCamera = std::make_shared<Camera>();
    mTestCube = std::make_shared<TestCube2>();
    mPointLight = std::make_shared<PointLight>();
    mDirectLight = std::make_shared<DirectLight>();
}

App::~App()
{
}

int App::run()
{
    std::shared_ptr<Program> PointLightShaderPtr = std::make_shared<Program>("Shader/PointLight_vs.vert", "Shader/PointLight_fs.frag");
    std::shared_ptr<Program> LightShaderPtr = std::make_shared<Program>("Shader/LightShader_vs.vert", "Shader/LightShader_fs.frag");
    std::shared_ptr<Program> NormalMapShaderPtr = std::make_shared<Program>("Shader/normalMap_vs.vert", "Shader/normalMap_fs.frag");

    std::shared_ptr<TestPlane> testPlane = std::make_shared<TestPlane>();

    //std::shared_ptr<Model> testModel = std::make_shared<Model>("Resource/Models/nono/nanosuit.obj");
    //std::shared_ptr<Model> testModel = std::make_shared<Model>("Resource/Models/Sponza/sponza.obj");

    float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    float lastFrame = 0.0f; // 上一帧的时间

    while (!glfwWindowShouldClose(mWindow.window()))
    {
        float aspect = (float)mWindow.getRectangle().first / (float)mWindow.getRectangle().second;
        mCamera->setAspect(aspect);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        handleInput(deltaTime);
        // clear buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        PointLightShaderPtr->activate();
        PointLightShaderPtr->setMatrix("view", mCamera->getView());
        PointLightShaderPtr->setMatrix("projection", mCamera->getProjection());
        mPointLight->draw(PointLightShaderPtr);

        /*
        LightShaderPtr->activate();
        // view / projection
        LightShaderPtr->setMatrix("view", mCamera->getView());
        LightShaderPtr->setMatrix("projection", mCamera->getProjection());
        // point light
        LightShaderPtr->setVec3("pointLight.posInView", glm::vec3{ mCamera->getView() * glm::vec4{ mPointLight->getPosition(), 1.0f } });
        LightShaderPtr->setVec3("pointLight.diffuse", mPointLight->getColor());
        LightShaderPtr->setFloat("pointLight.ambient", mPointLight->getAmbient());
        LightShaderPtr->setFloat("pointLight.specular", mPointLight->getSpecular());
        // direct light
        LightShaderPtr->setVec3("directLight.direction", glm::vec3{ mCamera->getView() * glm::vec4{ mDirectLight->getDirection(), 0.0f } });
        LightShaderPtr->setVec3("directLight.diffuse", mDirectLight->getColor());
        LightShaderPtr->setFloat("directLight.ambient", mDirectLight->getAmbient());
        LightShaderPtr->setFloat("directLight.specular", mDirectLight->getSpecular());
        // material
        LightShaderPtr->setFloat("material.shininess", 32.0f);
        //LightShaderPtr->setBool("hasSpec", true);
        // 设置采样器对应的纹理单元
        LightShaderPtr->setInt("material.diffuse", 0);
        LightShaderPtr->setInt("material.specular", 1);

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texDiffuse);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, texSepcular);
        // load shaderprogram
        //mTestCube->draw(LightShaderPtr);
        */

        NormalMapShaderPtr->activate();
        // vertex shader constant buffer
        NormalMapShaderPtr->setMatrix("view", mCamera->getView());
        NormalMapShaderPtr->setMatrix("projection", mCamera->getProjection());
        NormalMapShaderPtr->setVec3("PointLightPos", mPointLight->getPosition());
        NormalMapShaderPtr->setVec3("DirectLightDir", mDirectLight->getDirection());
        NormalMapShaderPtr->setVec3("viewPos", mCamera->getPosition());
        // pixel shader constant buffer
        // point light
        NormalMapShaderPtr->setVec3("pointLight.diffuse", mPointLight->getColor());
        NormalMapShaderPtr->setFloat("pointLight.ambient", mPointLight->getAmbient());
        NormalMapShaderPtr->setFloat("pointLight.specular", mPointLight->getSpecular());
        // direct light
        NormalMapShaderPtr->setVec3("directLight.diffuse", mDirectLight->getColor());
        NormalMapShaderPtr->setFloat("directLight.ambient", mDirectLight->getAmbient());
        NormalMapShaderPtr->setFloat("directLight.specular", mDirectLight->getSpecular());

        testPlane->draw(NormalMapShaderPtr);

        genCtrlGui();
        glfwSwapBuffers(mWindow.window());
        glfwPollEvents();
    }
    return 0;
}

void App::handleInput(float dt) noexcept
{
    // esc为窗口退出键
    if (glfwGetKey(mWindow.window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow.window(), true);

    if (!mWindow.isCursorEnabled()) {
        // handle input
        if (glfwGetKey(mWindow.window(), GLFW_KEY_W) == GLFW_PRESS)
            mCamera->translate({ 0.0f, 0.0f, dt });
        if (glfwGetKey(mWindow.window(), GLFW_KEY_S) == GLFW_PRESS)
            mCamera->translate({ 0.0f, 0.0f, -dt });
        if (glfwGetKey(mWindow.window(), GLFW_KEY_A) == GLFW_PRESS)
            mCamera->translate({ -dt, 0.0f, 0.0f });
        if (glfwGetKey(mWindow.window(), GLFW_KEY_D) == GLFW_PRESS)
            mCamera->translate({ dt, 0.0f, 0.0f });

        const auto cursorOffset = mWindow.getCursorOffset();
        mWindow.setCursorOffset(0.0f, 0.0f);
        mCamera->rotate(cursorOffset.first, cursorOffset.second);
    }
}

void App::genCtrlGui() const noexcept
{
    //创建imgui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool isShow = true;
    if(isShow)
        ImGui::ShowDemoWindow(&isShow);

    mCamera->genCtrlGui();
    mPointLight->genCtrlGui();
    mDirectLight->genCtrlGui();

    static bool isOpen = true;
    if (isOpen) {
        ImGui::Begin("test", &isOpen, ImGuiWindowFlags_MenuBar);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
