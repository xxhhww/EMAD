#include "Renderer.h"

#include "Program/Program.h"

#include "Object/Camera/Camera.h"
#include "Object/Light/PointLight.h"
#include "Object/Light/DirectLight.h"

#include "Object/Drawable/TestCube2.h"
#include "Object/Drawable/TestPlane.h"
#include "Object/Drawable/TestQuad.h"

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
	{
    // 启动深度缓冲测试
    glEnable(GL_DEPTH_TEST);
    // 启动模板测试
    glEnable(GL_STENCIL_TEST);
    // 在深度测试与模板测试都通过时，使用ref值(由glStencilFunc函数设置)代替模板缓冲中的值
    //glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

    std::vector<std::string> fileNames{
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };
    mSkyBox = std::make_shared<SkyBox>("Sky", "Resource/SkyBox", fileNames);
    mCamera = std::make_shared<Camera>();
    mPointLight = std::make_shared<PointLight>("PointLight1");
    mDirectLight = std::make_shared<DirectLight>();

    //布置场景
    mTestCube1 = std::make_shared<TestCube2>("Cube1");
    mTestCube1->setPosition(glm::vec3{ 2.0f, 0.0f, 0.0f });
    mTestCube1->setRotation(glm::vec3{ glm::radians(-45.0f), 0.0f, 0.0f });

    mTestCube2 = std::make_shared<TestCube2>("Cube2");
    mTestCube2->setPosition(glm::vec3{ 0.0f, -2.4f, 0.0f });
    //mTestCube2->setRotation(glm::vec3{ 0.0f, glm::radians(45.0f), 0.0f });

    mTestCube3 = std::make_shared<TestCube2>("Cube3");
    mTestCube3->setPosition(glm::vec3{ -2.0f, -0.9f, 1.0f });

    mTestPlane1 = std::make_shared<TestPlane>("Plane1");
    mTestPlane1->setPosition(glm::vec3{ 0.0f, -3.0f, 0.0f });
    mTestPlane1->setRotation(glm::vec3{ glm::radians(-90.0f), 0.0f, 0.0f });
    mTestPlane1->setScale(glm::vec3{ 10.0f, 10.0f, 10.0f });
}

App::~App()
{
}

int App::run()
{
    // 加载着色器
    std::shared_ptr<Program> PointLightShaderPtr = std::make_shared<Program>("Shader/PointLight_vs.vert", "Shader/PointLight_fs.frag");
    std::shared_ptr<Program> NormalMapShaderPtr = std::make_shared<Program>("Shader/normalMap_vs.vert", "Shader/normalMap_fs.frag");
    std::shared_ptr<Program> EdgeShaderPtr = std::make_shared<Program>("Shader/Edge_vs.vert", "Shader/Edge_fs.frag");
    std::shared_ptr<Program> NormalVisualShaderPtr =
        std::make_shared<Program>("Shader/NormalVisual_vs.vert", "Shader/NormalVisual_fs.frag", "Shader/NormalVisual_gs.geom");
    std::shared_ptr<Program> SkyBoxShaderPtr = std::make_shared<Program>("Shader/Skybox_vs.vert", "Shader/Skybox_fs.frag");

    std::shared_ptr<Program> GenDLDepthMapPtr = std::make_shared<Program>("Shader/GenDLDepthMap_vs.vert", "Shader/GenDLDepthMap_fs.frag");
    std::shared_ptr<Program> GenPLDepthMapPtr = std::make_shared<Program>("Shader/GenPLDepthMap_vs.vert", "Shader/GenPLDepthMap_fs.frag", "Shader/GenPLDepthMap_gs.geom");
    std::shared_ptr<Program> DebugQuadMapPtr = std::make_shared<Program>("Shader/DebugQuadMap_vs.vert", "Shader/DebugQuadMap_fs.frag");
    std::shared_ptr<Program> CubeShadowShaderPtr = std::make_shared<Program>("Shader/ShadowMapCube_vs.vert", "Shader/ShadowMapCube_fs.frag");

    // 
    std::shared_ptr<TestQuad> testQuad = std::make_shared<TestQuad>();

    // 将所有着色器的vpTrans(uniform块)设置为绑定点0
    unsigned int pointLightIndex = glGetUniformBlockIndex(PointLightShaderPtr->getProgram(), "vpTrans");
    unsigned int normalMapIndex = glGetUniformBlockIndex(NormalMapShaderPtr->getProgram(), "vpTrans");
    unsigned int edgeIndex = glGetUniformBlockIndex(EdgeShaderPtr->getProgram(), "vpTrans");
    unsigned int normalVisualIndex = glGetUniformBlockIndex(NormalVisualShaderPtr->getProgram(), "vpTrans");
    unsigned int skyboxIndex = glGetUniformBlockIndex(SkyBoxShaderPtr->getProgram(), "vpTrans");
    unsigned int cubeShadowIndex = glGetUniformBlockIndex(CubeShadowShaderPtr->getProgram(), "vpTrans");

    glUniformBlockBinding(PointLightShaderPtr->getProgram(), pointLightIndex, 0);
    glUniformBlockBinding(NormalMapShaderPtr->getProgram(), normalMapIndex, 0);
    glUniformBlockBinding(EdgeShaderPtr->getProgram(), edgeIndex, 0);
    glUniformBlockBinding(NormalVisualShaderPtr->getProgram(), normalVisualIndex, 0);
    glUniformBlockBinding(SkyBoxShaderPtr->getProgram(), skyboxIndex, 0);
    glUniformBlockBinding(CubeShadowShaderPtr->getProgram(), cubeShadowIndex, 0);
    
    // 创建并分配uniform缓冲对象
    unsigned int vpTrans;
    glGenBuffers(1, &vpTrans);
    glBindBuffer(GL_UNIFORM_BUFFER, vpTrans);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // 将uniform缓冲对象的特定范围绑定到绑定点0
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, vpTrans, 0, 2 * sizeof(glm::mat4));

    //std::shared_ptr<Model> testModel = std::make_shared<Model>("Resource/Models/nono/nanosuit.obj");
    //std::shared_ptr<Model> testModel = std::make_shared<Model>("Resource/Models/Sponza/sponza.obj");

    // 创建帧缓冲对象，用于存储深度缓冲纹理(在定向光的视角下)
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    // 创建2D纹理，用于存储深度缓冲的信息
    unsigned int SHADOW_WIDTH = 1024;
    unsigned int SHADOW_HEIGHT = 1024;
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    // 填充2D纹理对象信息
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // 创建立方体贴图，用来存储点光源的深度缓冲信息
    GLuint cubeDepthMap;
    glGenTextures(1, &cubeDepthMap);
    // 填充立方体贴图的属性
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeDepthMap);
    for (GLuint i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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
        
        // 第一步，获得从定向光角度看的深度缓冲
        // 此深度缓冲作为第二次渲染时的阴影映射
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        // 由于没有颜色缓冲附件的帧缓冲是不完整的，因此，我们需要告诉GL我们不使用任何颜色数据
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        // 定向光使用正交投影
        GLfloat near_plane = 1.0f, far_plane = 100.0f;
        glm::mat4 dlProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 dlView = glm::lookAt(-mDirectLight->getDirection(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 dlSpaceMat = dlProjection * dlView;

        GenDLDepthMapPtr->activate();
        GenDLDepthMapPtr->setMatrix("dlSpaceMat", dlSpaceMat);
        mTestCube1->draw(GenDLDepthMapPtr);
        mTestCube2->draw(GenDLDepthMapPtr);
        mTestCube3->draw(GenDLDepthMapPtr);
        mTestPlane1->draw(GenDLDepthMapPtr);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 第二步，获得从定向光角度看的深度缓冲
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeDepthMap, 0);

        // 由于没有颜色缓冲附件的帧缓冲是不完整的，因此，我们需要告诉GL我们不使用任何颜色数据
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glClear(GL_DEPTH_BUFFER_BIT);

        // 点光源使用正交投影
        GLfloat plAspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
        GLfloat near = 0.1f;
        GLfloat far = 100.0f;
        glm::mat4 plProjection = glm::perspective(glm::radians(90.0f), plAspect, near, far);
        // 因为要填充六个面，所以需要六个ViewTrans
        std::vector<glm::mat4> plSpaceMats;
        plSpaceMats.push_back(plProjection *
            glm::lookAt(mPointLight->getPosition(), mPointLight->getPosition() + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
        plSpaceMats.push_back(plProjection *
            glm::lookAt(mPointLight->getPosition(), mPointLight->getPosition() + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
        plSpaceMats.push_back(plProjection *
            glm::lookAt(mPointLight->getPosition(), mPointLight->getPosition() + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
        plSpaceMats.push_back(plProjection *
            glm::lookAt(mPointLight->getPosition(), mPointLight->getPosition() + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
        plSpaceMats.push_back(plProjection *
            glm::lookAt(mPointLight->getPosition(), mPointLight->getPosition() + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
        plSpaceMats.push_back(plProjection *
            glm::lookAt(mPointLight->getPosition(), mPointLight->getPosition() + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

        GenPLDepthMapPtr->activate();
        /*
        for (GLuint i = 0; i < 6; ++i)
            glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader.Program, 
                ("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, 
                glm::value_ptr(shadowTransforms[i]));
        */
        for (GLuint i = 0; i < 6; i++)
            GenPLDepthMapPtr->setMatrix(("plSpaceMats[" + std::to_string(i) + "]"), plSpaceMats[i]);
        GenPLDepthMapPtr->setVec3("plLightPos", mPointLight->getPosition());
        GenPLDepthMapPtr->setFloat("far_plane", far);
        mTestCube1->draw(GenPLDepthMapPtr);
        mTestCube2->draw(GenPLDepthMapPtr);
        mTestCube3->draw(GenPLDepthMapPtr);
        mTestPlane1->draw(GenPLDepthMapPtr);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 第三步，正常绘制
        glViewport(0, 0, (float)mWindow.getRectangle().first, (float)mWindow.getRectangle().second);
        //debug
        /*
        DebugQuadMapPtr->activate();
        DebugQuadMapPtr->setFloat("near_plane", near_plane);
        DebugQuadMapPtr->setFloat("far_plane", far_plane);
        testQuad->setTexture(depthMap);
        testQuad->draw(DebugQuadMapPtr);
        */
        
        SkyBoxShaderPtr->activate();
        SkyBoxShaderPtr->setMatrix("realView", glm::mat4(glm::mat3(mCamera->getView())));
        mSkyBox->draw(SkyBoxShaderPtr);

        PointLightShaderPtr->activate();
        mPointLight->setScale(glm::vec3{ 0.1f, 0.1f, 0.1f });
        mPointLight->draw(PointLightShaderPtr);
       
        CubeShadowShaderPtr->activate();
        // bind shadowMap
        CubeShadowShaderPtr->setInt("dlShadowMap", 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        // dlLightSpaceMatrix
        CubeShadowShaderPtr->setMatrix("dlSpaceMat", dlSpaceMat);
        // bind PL ShadowMap
        CubeShadowShaderPtr->setInt("plShadowMap", 3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeDepthMap);
        // bind far plane
        CubeShadowShaderPtr->setFloat("far_plane", far);
        // point light
        CubeShadowShaderPtr->setVec3("pointLight.posInView", glm::vec3{ mCamera->getView() * glm::vec4{ mPointLight->getPosition(), 1.0f} });
        CubeShadowShaderPtr->setVec3("pointLightPos", mPointLight->getPosition());
        CubeShadowShaderPtr->setVec3("pointLight.diffuse", mPointLight->getColor());
        CubeShadowShaderPtr->setFloat("pointLight.ambient", mPointLight->getAmbient());
        CubeShadowShaderPtr->setFloat("pointLight.specular", mPointLight->getSpecular());
        // direct light
        CubeShadowShaderPtr->setVec3("directLight.direction", glm::vec3{ mCamera->getView() * glm::vec4{ mDirectLight->getDirection(), 0.0f} });
        CubeShadowShaderPtr->setVec3("directLight.diffuse", mDirectLight->getColor());
        CubeShadowShaderPtr->setFloat("directLight.ambient", mDirectLight->getAmbient());
        CubeShadowShaderPtr->setFloat("directLight.specular", mDirectLight->getSpecular());

        mTestCube1->draw(CubeShadowShaderPtr);
        mTestCube2->draw(CubeShadowShaderPtr);
        mTestCube3->draw(CubeShadowShaderPtr);

        NormalMapShaderPtr->activate();
        // bind shadowMap
        NormalMapShaderPtr->setInt("dlShadowMap", 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        // dlLightSpaceMatrix
        NormalMapShaderPtr->setMatrix("dlSpaceMat", dlSpaceMat);
        // bind PL ShadowMap
        NormalMapShaderPtr->setInt("plShadowMap", 3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeDepthMap);
        // bind far plane
        NormalMapShaderPtr->setFloat("far_plane", far);
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

        mTestPlane1->draw(NormalMapShaderPtr);
        
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

    static bool isShow = false;
    if(isShow)
        ImGui::ShowDemoWindow(&isShow);

    mCamera->genCtrlGui();
    mPointLight->genCtrlGui();
    mDirectLight->genCtrlGui();
    mTestPlane1->genCtrlGui();
    mTestCube1->genCtrlGui();
    mTestCube2->genCtrlGui();
    mTestCube3->genCtrlGui();

    static bool isOpen = false;
    if (isOpen) {
        ImGui::Begin("test", &isOpen, ImGuiWindowFlags_MenuBar);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
