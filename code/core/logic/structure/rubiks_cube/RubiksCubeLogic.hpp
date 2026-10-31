#ifndef RUBIKS_CUBE_LOGIC_HPP
#define RUBIKS_CUBE_LOGIC_HPP

#include <vector>
#include <queue>
#include <functional>
#include <memory>
#include "Cubie.hpp"
#include "MathTypes.hpp"

struct MoveStep {
    MoveOption move;
    float angleRadians;
    bool wasAnimated = false; // האם המהלך בוצע עם אנימציה או לא
    bool recordHistory = true; // האם להוסיף את המהלך להיסטוריה או לא
    bool waitingToBeReversed = false;
};

struct AnimationState {
    MoveStep step;
    float progress = 0.0f; // ערך בין 0 ל-1 המייצג את התקדמות האנימציה
    float duration = 0.25f; // משך האנימציה בשניות
};

class RubiksCubeLogic {
private:
    std::vector<Cubie> staticCubies;
    std::vector<Cubie> animatedCubies;
    std::vector<MoveStep> stepHistory;
    std::queue<MoveStep> waitingMoves;

    // ניהול אנימציה פנימי בלוגיקה
    std::unique_ptr<AnimationState> currentAnimation;

    void fillAnimatedCubies(MoveOption move);
    MoveStep getCurrentAnimationStep() const;
    bool checkIsSolved() const;

    static MoveStep getInverseStep(const MoveStep& step);
    static MoveStep generateStep(MoveOption move, bool animated, bool recordHistory);
    
    static float getDuration(MoveOption move);
    static bool isCubieOnEntity(const Cubie& cubie, MoveOption entity);
    static Quaternion getEntityRotationAxis(const MoveStep& step);

    bool startAnimation(MoveStep step);

    bool isScrambling = false; // האם הקוביה נמצאת במצב ערבוב
    bool isAnimating = false; // האם הקוביה נמצאת במצב אנימציה
    bool isSolved = false; // האם הקוביה פתורה
public:
    RubiksCubeLogic();

    void applyLogicStep(const MoveStep& step);
    void undoLogicStep();

    void executeSequence(const std::vector<MoveOption>& sequence, bool animated = false, bool recordHistory = true);

    void scramble(int stepsCount = 20, bool animated = false, bool recordHistory = true);

    const std::vector<Cubie>& getStaticCubies() const;
    const std::vector<Cubie>& getAnimatedCubies() const;
    size_t getHistorySize() const;

    // מתודות חדשות לניהול האנימציה
    
    bool applyMoveAnimated(MoveOption move);
    bool reverseLastMoveAnimated();
    void updateAnimation(float deltaTime, bool *wasAnimating, bool *wasScrambling, bool *wasSolved);
    bool getIsAnimating() const;
    float getAnimationProgress() const;
    const AnimationState& getCurrentAnimation() const { return *currentAnimation; }
    bool isIdle() const { return (!getIsAnimating()) && waitingMoves.empty(); }
    bool getIsScrambling() const { return isScrambling; }
    glm::mat4 getCubieModelMatrix(const Cubie& cubie, bool isAnimated) const;
    bool getIsSolved() const { return isSolved; }
};

#endif // RUBIKS_CUBE_LOGIC_HPP