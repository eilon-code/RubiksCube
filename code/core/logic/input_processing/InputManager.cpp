#include "InputManager.hpp"
#include <iostream>

void InputManager::init(GLFWwindow* win, RubiksCubeLogic& logic, TimerUtil& t, CameraUtil& cam) {
    window = win;
    cubeLogic = &logic;
    timer = &t;
    camera = &cam;
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetWindowUserPointer(window, this);
}

// פונקציית העזר שמנהלת את ה-Debounce עבור כל מקש במפה הדינמית
bool InputManager::isKeyTriggered(int key) {
    bool& wasPressed = keyStates[key]; // יוצר או שולף אוטומטית (מתחיל ב-false)

    if (glfwGetKey(window, key) == GLFW_PRESS) {
        if (!wasPressed) {
            wasPressed = true;
            return true; // נלחץ כרגע לראשונה בפריים הזה!
        }
    } else {
        wasPressed = false; // המקש שוחרר
    }
    return false;
}

// --- מימוש פונקציות הטיפול המפורשות ---

void InputManager::handleEscape() {
    glfwSetWindowShouldClose(window, true);
}

void InputManager::handleRightMove() {
    cubeLogic->applyMoveAnimated(MoveOption::RIGHT);
    std::cout << "Starting Right face animation...\n";
}

void InputManager::handleUpMove() {
    cubeLogic->applyMoveAnimated(MoveOption::UP);
    std::cout << "Starting Upper face animation...\n";
}

void InputManager::handleLeftMove() {
    cubeLogic->applyMoveAnimated(MoveOption::LEFT);
    std::cout << "Starting Left face animation...\n";
}

void InputManager::handleDownMove() {
    cubeLogic->applyMoveAnimated(MoveOption::DOWN);
    std::cout << "Starting Down face animation...\n";
}

void InputManager::handleFrontMove() {
    cubeLogic->applyMoveAnimated(MoveOption::FRONT);
    std::cout << "Starting Front face animation...\n";
}

void InputManager::handleBackMove() {
    cubeLogic->applyMoveAnimated(MoveOption::BACK);
    std::cout << "Starting Back face animation...\n";
}

void InputManager::handleScramble() {
    std::cout << "Scrambling cube...\n";
    cubeLogic->scramble(20, true, true);
    timer->reset();
}

void InputManager::handleUndo() {
    // TODO: fix here
    cubeLogic->undoLogicStep();
    std::cout << "Undoing last move...\n";
}

// --- הפונקציה הראשית שרצה בלולאה ומפעילה את הפונקציות לפי שמותיהן ---
void InputManager::processLogicInput() {
    if (!window || !cubeLogic || !timer) return;

    if (isKeyTriggered(GLFW_KEY_ESCAPE)) handleEscape();
    if (isKeyTriggered(GLFW_KEY_R))      handleRightMove();
    if (isKeyTriggered(GLFW_KEY_U))      handleUpMove();
    if (isKeyTriggered(GLFW_KEY_L))      handleLeftMove();
    if (isKeyTriggered(GLFW_KEY_D))      handleDownMove();
    if (isKeyTriggered(GLFW_KEY_F))      handleFrontMove();
    if (isKeyTriggered(GLFW_KEY_B))      handleBackMove();
    if (isKeyTriggered(GLFW_KEY_S))      handleScramble();
    if (isKeyTriggered(GLFW_KEY_Z))      handleUndo();
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
    InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!manager) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            manager->isMousePressed = true;
            glfwGetCursorPos(window, &manager->lastMouseX, &manager->lastMouseY);
        } else if (action == GLFW_RELEASE) {
            manager->isMousePressed = false;
        }
    }
}

void InputManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!manager || !manager->isMousePressed) return;

    float xoffset = static_cast<float>(xpos - manager->lastMouseX);
    float yoffset = static_cast<float>(manager->lastMouseY - ypos);
    manager->lastMouseX = xpos;
    manager->lastMouseY = ypos;

    float sensitivity = 0.22f;

    manager->camera->processMouseMovement(xoffset, yoffset, sensitivity);
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    [[maybe_unused]] double unused_x = xoffset; // השקטת אזהרת משתנה לא בשימוש
    InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!manager) return;

    float zoomSpeed = 0.5f;
    manager->camera->processMouseScroll(yoffset, zoomSpeed);
}
