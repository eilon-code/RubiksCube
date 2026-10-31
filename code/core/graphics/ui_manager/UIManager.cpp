#include "UIManager.hpp"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <functional>

UIManager::~UIManager()
{
    // --- ניקוי משאבי ImGui ביציאה ---
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::init(GLFWwindow *win, TimerUtil &t, CameraUtil &cam, RubiksCubeLogic &logic)
{
    window = win;
    timer = &t;
    camera = &cam;
    cubeLogic = &logic;
    shaderManager = std::make_unique<ShadersManager>("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    cubieRenderer = std::make_unique<CubieRenderer>();

    // --- אתחול ImGui (חובה כדי למנוע קריסת Access Violation) ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // אתחול ה־Backends של GLFW ו־OpenGL3
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330"); // או גרסת ה-GLsl שלך
    // -----------------------------------------------------------
}

void initGUI()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void initIMGUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void flushIMGUI() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void UIManager::render() {
    if (!window || !timer || !cubeLogic || !camera) return;
    
    initGUI();
    initIMGUI();

    renderRubiksCube();
    renderTimerHUD();
    renderControlsHUD();

    flushIMGUI();
}

void UIManager::renderRubiksCube() {
    if (!(shaderManager && cubieRenderer && camera && cubeLogic)) return;

    shaderManager->use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = camera->getViewMatrix();

    shaderManager->setMat4("view", view);
    shaderManager->setMat4("projection", projection);

    const auto& staticCubies = cubeLogic->getStaticCubies();
    for (const auto& cubie : staticCubies) {
        glm::mat4 model = cubeLogic->getCubieModelMatrix(cubie, false);

        shaderManager->setMat4("model", model);
        cubieRenderer->draw(cubie); // העברת ה-Cubie הישיר לציור
    }

    if (!cubeLogic->getIsAnimating()) return;

    const auto& animatedCubies = cubeLogic->getAnimatedCubies();
    for (const auto& cubie : animatedCubies) {
        glm::mat4 model = cubeLogic->getCubieModelMatrix(cubie, true);
        shaderManager->setMat4("model", model);
        cubieRenderer->draw(cubie); // העברת ה-Cubie הישיר לציור
    }
}

void UIManager::renderTimerHUD() {
    int displayW, displayH;
    glfwGetFramebufferSize(window, &displayW, &displayH);

    // מיקום בחלק העליון במרכז
    ImGui::SetNextWindowPos(ImVec2(displayW * 0.5f - 75.0f, 15.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(150.0f, 50.0f), ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | 
                             ImGuiWindowFlags_NoMove | 
                             ImGuiWindowFlags_NoSavedSettings | 
                             ImGuiWindowFlags_NoFocusOnAppearing | 
                             ImGuiWindowFlags_NoNav;

    ImGui::Begin("TimerHUD", nullptr, flags);
    
    double elapsed = timer->getElapsedSeconds();
    int minutes = static_cast<int>(elapsed) / 60;
    int seconds = static_cast<int>(elapsed) % 60;
    int millis = static_cast<int>((elapsed - static_cast<int>(elapsed)) * 1000);

    // עיצוב הטקסט (טורקיז זוהר)
    ImGui::SetCursorPosX(15.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.8f, 1.0f));
    ImGui::Text("%02d:%02d.%03d", minutes, seconds, millis);
    ImGui::PopStyleColor();

    ImGui::End();
}

void UIManager::renderControlsHUD() {
    // מיקום בצד שמאל למעלה
    ImGui::SetNextWindowPos(ImVec2(15.0f, 70.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(130.0f, 380.0f), ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | 
                             ImGuiWindowFlags_NoMove | 
                             ImGuiWindowFlags_NoSavedSettings;

    ImGui::Begin("ControlsHUD", nullptr, flags);
    ImGui::Text("Controls");
    ImGui::Separator();

    // פונקציית עזר לציור כפתור שמגיב גם למקלדת וגם לעכבר
    auto drawKeyButton = [this](const char* label, int glfwKey, std::function<void()> onClick) {
        bool isKeyPressed = (glfwGetKey(window, glfwKey) == GLFW_PRESS);

        if (isKeyPressed) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.5f, 0.0f, 1.0f)); // צבע כתום בלחיצה
        }

        bool clickedByMouse = ImGui::Button(label, ImVec2(100.0f, 30.0f));

        if (isKeyPressed) {
            ImGui::PopStyleColor();
        }

        // הפעלת הפעולה אם לחצו בעכבר או במקלדת
        if (clickedByMouse || isKeyPressed) {
            onClick();
        }
    };

    // הצגת הכפתורים עם קישור לפעולות הלוגיות
    drawKeyButton("R [Right]", GLFW_KEY_R, [this]() { cubeLogic->applyMoveAnimated(MoveOption::RIGHT); });
    drawKeyButton("U [Upper]", GLFW_KEY_U, [this]() { cubeLogic->applyMoveAnimated(MoveOption::UP); });
    drawKeyButton("L [Left]",  GLFW_KEY_L, [this]() { cubeLogic->applyMoveAnimated(MoveOption::LEFT); });
    drawKeyButton("D [Down]",  GLFW_KEY_D, [this]() { cubeLogic->applyMoveAnimated(MoveOption::DOWN); });
    drawKeyButton("F [Front]", GLFW_KEY_F, [this]() { cubeLogic->applyMoveAnimated(MoveOption::FRONT); });
    drawKeyButton("B [Back]",  GLFW_KEY_B, [this]() { cubeLogic->applyMoveAnimated(MoveOption::BACK); });
    
    ImGui::Separator();
    drawKeyButton("S [Scramble]", GLFW_KEY_S, [this]() { 
        cubeLogic->scramble(20, true, true); 
        timer->reset(); 
    });
    drawKeyButton("Z [Undo]",     GLFW_KEY_Z, [this]() { cubeLogic->undoLogicStep(); });

    ImGui::End();
}
