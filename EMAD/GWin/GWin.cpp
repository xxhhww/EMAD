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

    //�����ڳ���ֱ������Ϊ��ǰ�߳�������
    glfwMakeContextCurrent(mWindow);
    //���ô��ڴ�С�ı�ʱ�Ļص�����
    glfwSetFramebufferSizeCallback(mWindow,
        [](GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
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
