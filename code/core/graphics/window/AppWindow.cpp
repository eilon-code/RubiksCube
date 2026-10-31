#include "AppWindow.hpp"
#include <iostream>

AppWindow::AppWindow(int w, int h, const std::string& t) 
    : width(w), height(h), title(t), window(nullptr) {}

AppWindow::~AppWindow() {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

bool AppWindow::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << "\n";
        return false;
    }
    glEnable(GL_DEPTH_TEST);

    return true;
}

bool AppWindow::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void AppWindow::swapBuffers() const {
    glfwSwapBuffers(window);
}

void AppWindow::pollEvents() const {
    glfwPollEvents();
}

void AppWindow::framebufferSizeCallback(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}
