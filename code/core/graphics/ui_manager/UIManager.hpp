#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include "AppWindow.hpp"
#include "ShadersManager.hpp"
#include "CubieRenderer.hpp" // מחליף את TriangleRenderer
#include "CameraUtil.hpp"
#include "TimerUtil.hpp"
#include "RubiksCubeLogic.hpp"

class UIManager {
private:
    GLFWwindow* window;
    CameraUtil* camera;
    TimerUtil* timer;
    RubiksCubeLogic* cubeLogic;

    std::unique_ptr<ShadersManager> shaderManager;
    std::unique_ptr<CubieRenderer> cubieRenderer; // מחליף את TriangleRenderer

    void renderRubiksCube();
    void renderTimerHUD();
    void renderControlsHUD();

public:
    UIManager() = default;
    ~UIManager();

    // אתחול המנהל עם כל התלויות הנדרשות
    void init(GLFWwindow* win, TimerUtil& t, CameraUtil& cam, RubiksCubeLogic& logic);

    // הפונקציה הראשית שנקראת מתוך לולאת הרינדור הראשית (Main Loop)
    void render();
};

#endif // UI_MANAGER_HPP
