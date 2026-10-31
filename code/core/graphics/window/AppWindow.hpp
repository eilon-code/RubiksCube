#pragma once

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

class AppWindow {
public:
    AppWindow(int width, int height, const std::string& title);
    ~AppWindow();

    bool init();
    bool shouldClose() const;
    void swapBuffers() const;
    void pollEvents() const;
    GLFWwindow* getNativeWindow() const { return window; }

private:
    int width, height;
    std::string title;
    GLFWwindow* window;

    static void framebufferSizeCallback(GLFWwindow* window, int w, int h);
};
