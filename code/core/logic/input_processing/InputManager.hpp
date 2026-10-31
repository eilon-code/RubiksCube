#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

// #include <GLFW/glfw3.h>
#include <unordered_map>
#include "AppWindow.hpp"
#include "CameraUtil.hpp"
#include "TimerUtil.hpp"
#include "RubiksCubeLogic.hpp"

class InputManager {
private:
    GLFWwindow* window;
    RubiksCubeLogic* cubeLogic;
    CameraUtil* camera;
    TimerUtil* timer;

    // מפה דינמית ששומרת את מצב הלחיצה הקודם של המקשים
    std::unordered_map<int, bool> keyStates;

    // נתוני עכבר
    bool isMousePressed = false;
    double lastMouseX = 0.0, lastMouseY = 0.0;

    // פונקציית עזר בסיסית שמטפלת ב-Debounce ומחזירה true רק בלחיצה חדשה
    bool isKeyTriggered(int key);

    // --- פונקציות טיפול ייעודיות עם שמות ברורים ---
    void handleEscape();
    void handleRightMove();
    void handleUpMove();
    void handleLeftMove();
    void handleDownMove();
    void handleFrontMove();
    void handleBackMove();
    void handleScramble();
    void handleUndo();

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
public:
    InputManager() = default;
    
    void init(GLFWwindow* win, RubiksCubeLogic& logic, TimerUtil& t, CameraUtil& camera);
    void processLogicInput();
};

#endif // INPUT_MANAGER_HPP
