#include "Renderer.h"

#include "Shader/ShaderProgram.h"

#include "Object/Camera/Camera.h"
#include "Object/Light/PointLight.h"
#include "Object/Light/DirectLight.h"

#include "Object/Drawable/TestCube.h"
#include "Object/Drawable/TestCube2.h"

#include "stb_image.h"
#include "Exception.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
    mDirectLight = std::make_shared<DirectLight>();

    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile("Resource/Models/suzanne.obj",
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

    std::cout << scene->mMeshes[0]->mNumVertices << std::endl;
}

App::~App()
{
}

int App::run()
{
    std::shared_ptr<Program> LightShaderPtr = std::make_shared<Program>("Shader/LightShader_vs.vert", "Shader/LightShader_fs.frag");

    float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    float lastFrame = 0.0f; // 上一帧的时间

    // 加载材质信息
    unsigned int texDiffuse, texSepcular;
    glGenTextures(1, &texDiffuse);
    glGenTextures(1, &texSepcular);
    // bind texDiffuse
    glBindTexture(GL_TEXTURE_2D, texDiffuse);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载图片并生成纹理
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("Resource/wooden_diffuse.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        THROW_INFO_EXCEPTION("Failed to create texture2D");
    }
    stbi_image_free(data);
    // bind texture 2
    glBindTexture(GL_TEXTURE_2D, texSepcular);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载图片并生成纹理
    data = stbi_load("Resource/wooden_specular.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        THROW_INFO_EXCEPTION("Failed to create texture2D");
    }
    stbi_image_free(data);

    // 设置采样器对应的纹理单元
    LightShaderPtr->activate();
    LightShaderPtr->setInt("material.diffuse", 0);
    LightShaderPtr->setInt("material.specular", 1);


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

        mPointLight->draw(mCamera->getView(), mCamera->getProjection());
        LightShaderPtr->activate();
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
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texDiffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texSepcular);
        // load shaderprogram
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
    mDirectLight->genCtrlGui();
    mTestCube->genCtrlGui();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
