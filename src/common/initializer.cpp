#include <initializer.h>
#include <iostream>

Renderer::Renderer() {}

void Renderer::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
}

void Renderer::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* Renderer::createWindow() {
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    //Create an OpenGL contex for the window
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //Limit framerate
    return window;
}

void Renderer::initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, windowWidth, windowHeight);
}