#include "Renderer.h"
#include "Object/Geometry/TestCube.h"
#include "Object/Camera/Camera.h"

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
}

App::~App()
{
}

int App::run()
{

    glfwSetInputMode(mWindow.window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Camera testCamera;
    TestCube testCube;
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    float lastFrame = 0.0f; // 上一帧的时间
    while (!glfwWindowShouldClose(mWindow.window()))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // handle input
        if (glfwGetKey(mWindow.window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow.window(), true);
        if (glfwGetKey(mWindow.window(), GLFW_KEY_W) == GLFW_PRESS)
            testCamera.translate({ 0.0f, 0.0f, -deltaTime });
        if (glfwGetKey(mWindow.window(), GLFW_KEY_S) == GLFW_PRESS)
            testCamera.translate({0.0f, 0.0f, deltaTime});
        if (glfwGetKey(mWindow.window(), GLFW_KEY_A) == GLFW_PRESS)
            testCamera.translate({ -deltaTime, 0.0f, 0.0f });
        if (glfwGetKey(mWindow.window(), GLFW_KEY_D) == GLFW_PRESS)
            testCamera.translate({ deltaTime, 0.0f, 0.0f });
        const auto cursorOffset = mWindow.getCursorOffset();
        mWindow.setCursorOffset(0.0f, 0.0f);
        testCamera.rotate(cursorOffset.first, cursorOffset.second);

        // clear buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (size_t i = 0; i < 10; i++) {
            // 绑定顶点着色器的常量缓存
            // model trans
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
            // projection trans
            float aspect = (float)mWindow.getRectangle().first / (float)mWindow.getRectangle().second;  
            testCamera.setAspect(aspect);
            testCube.draw(model, testCamera.getView(), testCamera.getProjection());
        }

        //创建imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool isShow = true;
        ImGui::ShowDemoWindow(&isShow);
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(mWindow.window());
        glfwPollEvents();
    }
    return 0;
}
