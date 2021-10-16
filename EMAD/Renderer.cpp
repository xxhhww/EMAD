#include "Renderer.h"
#include "Object/Geometry/TestCube.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

App::App(int width, int height, const std::string& name) noexcept
	:mWindow(width, height, name){}

App::~App()
{
}

int App::run()
{
    TestCube testCube;

    while (!glfwWindowShouldClose(mWindow.window()))
    {
        // input
        if (glfwGetKey(mWindow.window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow.window(), true);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 绑定顶点着色器的常量缓存
        // model trans
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        // view trans
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // projection trans
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        testCube.draw(glm::value_ptr(model), glm::value_ptr(view), glm::value_ptr(projection));

        glfwSwapBuffers(mWindow.window());
        glfwPollEvents();
    }
    return 0;
}
