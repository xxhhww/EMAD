#include "GWin.h"
#include "../Exception.h"

#include <iostream>

GWin::GWin(int width, int height, const std::string& name)
    :mWidth(width)
    ,mHeight(height)
    ,mName(name)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), NULL, NULL);
    if (mWindow == nullptr){
        THROW_INFO_EXCEPTION("Failed to create GLFW window");
    }

    // 单窗口程序，直接设置为当前线程上下文
    glfwMakeContextCurrent(mWindow);
    // 设置用户数据指针
    glfwSetWindowUserPointer(mWindow, this);
    // 设置窗口大小改变时的回调函数
    glfwSetFramebufferSizeCallback(mWindow,
        [](GLFWwindow* window, int width, int height) {
            // 改变OpenGL视口变换矩阵
            glViewport(0, 0, width, height);
            GWin* userData = (GWin*)glfwGetWindowUserPointer(window);
            userData->setRectangle(width, height);
        }
    );

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        THROW_INFO_EXCEPTION("Failed to initialize GLAD");
    }
}

GWin::~GWin()
{
    glfwTerminate();
}

std::pair<int, int> GWin::getRectangle() const noexcept
{
    return std::pair<int, int>(mWidth, mHeight);
}

void GWin::setRectangle(int width, int height) noexcept
{
    mWidth = width;
    mHeight = height;
}
