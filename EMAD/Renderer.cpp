#include "Renderer.h"

#include "Shader/ShaderProgram.h"

#include "Object/Camera/Camera.h"
#include "Object/Light/PointLight.h"

#include "Object/Drawable/TestCube.h"
#include "Object/Drawable/TestCube2.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

App::App(int width, int height, const std::string& name) noexcept
	:mWindow(width, height, name){
    // 启东深度缓冲测试
    glEnable(GL_DEPTH_TEST);

    mCamera = std::make_shared<Camera>();
    mTestCube = std::make_shared<TestCube2>();
    mPointLight = std::make_shared<PointLight>();
}

App::~App()
{
}

int App::run()
{
    std::shared_ptr<Program> LightShaderPtr = std::make_shared<Program>("Shader/LightShader_vs.vert", "Shader/LightShader_fs.frag");

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

        glm::vec3 objectColor{ 0.3f, 0.7f, 0.4f };

        mPointLight->draw(mCamera->getView(), mCamera->getProjection());
        LightShaderPtr->activate();
        LightShaderPtr->setVec3("lightPosInView", mCamera->getView() * glm::vec4{ mPointLight->getPosition(), 1.0f });
        LightShaderPtr->setVec3("lightColor", mPointLight->getColor());
        LightShaderPtr->setFloat("lightAmbient", mPointLight->getAmbient());
        LightShaderPtr->setFloat("lightSpecular", mPointLight->getSpecular());
        LightShaderPtr->setVec3("objectColor", objectColor);

        mTestCube->loadShaderProgram(LightShaderPtr);
        mTestCube->draw(mCamera->getView(), mCamera->getProjection());

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

    //bool isShow = true;
    //ImGui::ShowDemoWindow(&isShow);

    mCamera->genCtrlGui();
    mPointLight->genCtrlGui();
    mTestCube->genCtrlGui();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
