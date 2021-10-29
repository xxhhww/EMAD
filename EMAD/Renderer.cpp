#include "Renderer.h"

#include "Shader/ShaderProgram.h"

#include "Object/Camera/Camera.h"
#include "Object/Light/PointLight.h"
#include "Object/Light/DirectLight.h"

#include "Object/Drawable/TestCube.h"
#include "Object/Drawable/TestCube2.h"
#include "Object/Drawable/TestPlane.h"
#include "Object/Drawable/SkyBox.h"
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
    // 启动深度缓冲测试
    glEnable(GL_DEPTH_TEST);
    // 启动模板测试
    glEnable(GL_STENCIL_TEST);
    // 在深度测试与模板测试都通过时，使用ref值(由glStencilFunc函数设置)代替模板缓冲中的值
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    mCamera = std::make_shared<Camera>();
    mTestCube = std::make_shared<TestCube2>();
    mPointLight = std::make_shared<PointLight>();
    mDirectLight = std::make_shared<DirectLight>();

    std::vector<std::string> fileNames{
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };

    mSkyBox = std::make_shared<SkyBox>("Resource/SkyBox", fileNames);
}

App::~App()
{
}

int App::run()
{
    std::shared_ptr<Program> PointLightShaderPtr = std::make_shared<Program>("Shader/PointLight_vs.vert", "Shader/PointLight_fs.frag");
    std::shared_ptr<Program> NormalMapShaderPtr = std::make_shared<Program>("Shader/normalMap_vs.vert", "Shader/normalMap_fs.frag");
    std::shared_ptr<Program> EdgeShaderPtr = std::make_shared<Program>("Shader/Edge_vs.vert", "Shader/Edge_fs.frag");
    std::shared_ptr<Program> SkyBoxShaderPtr = std::make_shared<Program>("Shader/Skybox_vs.vert", "Shader/Skybox_fs.frag");

    // 将所有着色器的vpTrans(uniform块)设置为绑定点0
    unsigned int pointLightIndex = glGetUniformBlockIndex(PointLightShaderPtr->getProgram(), "vpTrans");
    unsigned int normalMapIndex = glGetUniformBlockIndex(NormalMapShaderPtr->getProgram(), "vpTrans");
    unsigned int edgeIndex = glGetUniformBlockIndex(EdgeShaderPtr->getProgram(), "vpTrans");
    unsigned int skyboxIndex = glGetUniformBlockIndex(SkyBoxShaderPtr->getProgram(), "vpTrans");

    glUniformBlockBinding(PointLightShaderPtr->getProgram(), pointLightIndex, 0);
    glUniformBlockBinding(NormalMapShaderPtr->getProgram(), normalMapIndex, 0);
    glUniformBlockBinding(EdgeShaderPtr->getProgram(), edgeIndex, 0);
    glUniformBlockBinding(SkyBoxShaderPtr->getProgram(), skyboxIndex, 0);
    
    // 创建并分配uniform缓冲对象
    unsigned int vpTrans;
    glGenBuffers(1, &vpTrans);
    glBindBuffer(GL_UNIFORM_BUFFER, vpTrans);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // 将uniform缓冲对象的特定范围绑定到绑定点0
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, vpTrans, 0, 2 * sizeof(glm::mat4));

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // 填充uniform缓冲对象(vpTrans)
        glBindBuffer(GL_UNIFORM_BUFFER, vpTrans);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mCamera->getView()));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mCamera->getProjection()));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        SkyBoxShaderPtr->activate();
        SkyBoxShaderPtr->setMatrix("realView", glm::mat4(glm::mat3(mCamera->getView())));
        mSkyBox->draw(SkyBoxShaderPtr);

        PointLightShaderPtr->activate();
        mPointLight->draw(PointLightShaderPtr);

        NormalMapShaderPtr->activate();
        // vertex shader constant buffer
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

        EdgeShaderPtr->activate();
        // vertex shader constant buffer

        // 绘制原物体，将模板缓冲中对应的区域绘制为1
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        testPlane->setScale(glm::vec3{ 1.0f, 1.0f, 1.0f });
        testPlane->draw(NormalMapShaderPtr);
        // 绘制放大版的物体，作为原物体的边缘
        // 如果模板缓冲中的值不等于1，才算通过模板测试，才能有机会绘制在屏幕上
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        // 关闭模板缓冲的写入
        glStencilMask(0x00);
        // 关闭深度测试(防止被其他物体覆盖)
        glDisable(GL_DEPTH_TEST);
        testPlane->setScale(glm::vec3{ 1.01f, 1.01f, 1.01f });
        testPlane->draw(EdgeShaderPtr);
        // 重新启动深度缓冲与模板缓冲
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);

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
