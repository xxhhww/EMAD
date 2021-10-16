#include "Renderer.h"
#include "Object/Geometry/TestCube.h"

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

    while (!glfwWindowShouldClose(mWindow.window()))
    {
        // handle input
        if (glfwGetKey(mWindow.window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow.window(), true);

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
            // view trans
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            // projection trans
            glm::mat4 projection;
            float aspect = (float)mWindow.getRectangle().first / (float)mWindow.getRectangle().second;
            projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

            testCube.draw(glm::value_ptr(model), glm::value_ptr(view), glm::value_ptr(projection));
        }

        glfwSwapBuffers(mWindow.window());
        glfwPollEvents();
    }
    return 0;
}
