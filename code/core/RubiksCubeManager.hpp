#pragma once

#include <memory>
#include "AppWindow.hpp"
#include "CameraUtil.hpp"
#include "TimerUtil.hpp"
#include "RubiksCubeLogic.hpp"
#include "InputManager.hpp"
#include "UIManager.hpp"

class RubiksCubeManager {
public:
    RubiksCubeManager(int width, int height, const std::string& title);
    ~RubiksCubeManager();

    void run(bool debug = false); // פרמטר אופציונלי להפעלת מצב דיבאג
    
private:
    void processInput();
    void update(float deltaTime = 0.016f); // ברירת מחדל של 60 FPS
    void render();
    
    AppWindow window;
    CameraUtil camera;
    TimerUtil timerPostScramble;
    RubiksCubeLogic cubeLogic;
    InputManager inputManager;
    UIManager uiManager;
};
