#include "TimerUtil.hpp"
#include <GLFW/glfw3.h>

void TimerUtil::start() {
    double currentTime = glfwGetTime();
    isActive = true;
    timerStarted = true;
    startTime = currentTime;
    elapsedSeconds = 0.0;
}

void TimerUtil::stop() {
    isActive = false;
}

void TimerUtil::reset() {
    isActive = false;
    timerStarted = false;
    startTime = 0.0;
    elapsedSeconds = 0.0;
}

void TimerUtil::update() {
    double currentTime = glfwGetTime();
    if (isActive) {
        elapsedSeconds = currentTime - startTime;
    }
}
