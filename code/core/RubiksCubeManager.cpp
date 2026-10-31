#include "RubiksCubeManager.hpp"
#include <iostream>

RubiksCubeManager::RubiksCubeManager(int width, int height, const std::string& title)
    : window(width, height, title) {}

RubiksCubeManager::~RubiksCubeManager() = default;

void RubiksCubeManager::run(bool debug) {
    // 1. קודם כל מאתחלים את החלון ו־GLEW
    if (!window.init()) {
        std::cerr << "Failed to initialize AppWindow!\n";
        return;
    }

    // 2. רק עכשיו, כשיש קונטקסט OpenGL פעיל ובטוח, יוצרים את השיידרים והרנדרר
    // (שים לב לנתיב הנכון ביחס לתיקיית ההרצה, למשל מתוך build התיקייה נמצאת ב־../shaders)
    inputManager.init(window.getNativeWindow(), cubeLogic, timerPostScramble, camera);
    uiManager.init(window.getNativeWindow(), timerPostScramble, camera, cubeLogic);

    std::cout << "Rubik's Cube Logic initialized with " << cubeLogic.getStaticCubies().size() << " cubies.\n";

    double lastTime = glfwGetTime();

    // 3. לולאת הרינדור הראשית
    while (!window.shouldClose()) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        if (debug) {
            std::cout << "[DEBUG] Processing input..." << std::endl;
        }
        processInput();
        if (debug) {
            std::cout << "[DEBUG] Updating..." << std::endl;
        }
        update(deltaTime);
        if (debug) {
            std::cout << "[DEBUG] Rendering..." << std::endl;
        }
        render();
        if (debug) {
            std::cout << "[DEBUG] Iteration completed." << std::endl;
        }

        window.swapBuffers();
        window.pollEvents();
    }
}

void RubiksCubeManager::processInput() {
    inputManager.processLogicInput();
}

void RubiksCubeManager::update(float deltaTime) {
    bool wasAnimating = false;
    bool wasScrambling = false;
    bool wasSolved = false;
    cubeLogic.updateAnimation(deltaTime, &wasAnimating, &wasScrambling, &wasSolved);

    bool cubeJustSolved = false;
    if (cubeLogic.isIdle()) {
        if (wasScrambling) {
            timerPostScramble.start();
            std::cout << "Scramble finished! Timer started.\n";
        }
        else if (wasAnimating) {
            std::cout << "Animation finished.\n";
            // בדיקה האם הקוביה נפתרה!
            cubeJustSolved = cubeLogic.getIsSolved() && !wasSolved;
            std::cout << "Cube just got solved? " << (cubeJustSolved ? "Yes" : "No") << std::endl;
            std::cout << "number of static cubies: " << cubeLogic.getStaticCubies().size() << std::endl;
        }
    }

    if (cubeJustSolved) {
        double elapsed = timerPostScramble.getElapsedSeconds();
        std::cout << "Cube solved! Final Time: " << elapsed << " seconds.\n";
        timerPostScramble.stop(); // עוצרים את הטיימר
    }

    // עדכון הטיימר ובדיקת פתרון
    if (timerPostScramble.getIsActive()) {
        timerPostScramble.update();
        double elapsed = timerPostScramble.getElapsedSeconds();

        // הצגת הזמן בכותרת החלון
        std::string str = std::to_string(static_cast<int>(elapsed * 1000) / 1000.0);
        std::string titleStr = "Rubik's Cube Simulator | Time: " + str + "s";
        glfwSetWindowTitle(window.getNativeWindow(), titleStr.c_str());
    }
}

void RubiksCubeManager::render() {
    uiManager.render();
}
