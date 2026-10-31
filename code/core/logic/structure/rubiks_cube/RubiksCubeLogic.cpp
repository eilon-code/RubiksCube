#include "RubiksCubeLogic.hpp"
#include <cmath>
#include <random> // עבור מספרים אקראיים לערבוב
#include <glm/gtx/quaternion.hpp>

RubiksCubeLogic::RubiksCubeLogic() : currentAnimation(nullptr) {
    staticCubies = std::vector<Cubie>();
    animatedCubies = std::vector<Cubie>();
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                staticCubies.emplace_back(
                    Vector3d(x, y, z),
                    Quaternion(1.0f, 0.0f, 0.0f, 0.0f)
                );
            }
        }
    }
}

bool RubiksCubeLogic::isCubieOnEntity(const Cubie &cubie, MoveOption entity)
{
    switch (entity) {
        case MoveOption::UP:    return std::abs(cubie.position.z -  1.0f) < 0.01f;
        case MoveOption::DOWN:  return std::abs(cubie.position.z - (-1.0f)) < 0.01f;
        case MoveOption::RIGHT: return std::abs(cubie.position.x -  1.0f) < 0.01f;
        case MoveOption::LEFT:  return std::abs(cubie.position.x - (-1.0f)) < 0.01f;
        case MoveOption::FRONT: return std::abs(cubie.position.y -  1.0f) < 0.01f;
        case MoveOption::BACK:  return std::abs(cubie.position.y - (-1.0f)) < 0.01f;
        default:
            break;
    }
    return false;
}

Quaternion RubiksCubeLogic::getEntityRotationAxis(const MoveStep& step)
{
    switch (step.move) {
        case MoveOption::UP:    
        case MoveOption::DOWN:  
            return glm::angleAxis(step.angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        case MoveOption::RIGHT: 
        case MoveOption::LEFT:  
            return glm::angleAxis(step.angleRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        case MoveOption::FRONT: 
        case MoveOption::BACK:  
            return glm::angleAxis(step.angleRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        
        default:
            break;
    }
    return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

void RubiksCubeLogic::fillAnimatedCubies(MoveOption move) {
    for (auto it = staticCubies.begin(); it != staticCubies.end(); ) {
        if (RubiksCubeLogic::isCubieOnEntity(*it, move)) {
            animatedCubies.push_back(*it);
            it = staticCubies.erase(it);
        } else {
            ++it;
        }
    }
}

MoveStep RubiksCubeLogic::getInverseStep(const MoveStep &step)
{
    return {
        step.move,
        -step.angleRadians,
        step.wasAnimated,
        false, // לא להוסיף להיסטוריה,
        true, // marked as being reversed
    };
}

MoveStep RubiksCubeLogic::generateStep(MoveOption move, bool animated, bool recordHistory)
{
    return { move, glm::radians(90.0f), animated, recordHistory };
}

float RubiksCubeLogic::getDuration(MoveOption move) {
    switch (move)
    {
    case MoveOption::UP:
    case MoveOption::DOWN:
        return 0.45f;
    case MoveOption::RIGHT:
    case MoveOption::LEFT:
        return 0.45f;
    case MoveOption::FRONT:
    case MoveOption::BACK:
        return 0.45f;

    default:
        break;
    }
    return 0.45f; // משך אנימציה קבוע לכל המהלכים
}

void RubiksCubeLogic::applyLogicStep(const MoveStep& step)
{
    Quaternion rotQuat = getEntityRotationAxis(step);
    glm::mat3 rotMatrix = glm::toMat3(rotQuat);

    if (!step.wasAnimated) {
        fillAnimatedCubies(step.move);
    }
    for (auto& cubie : animatedCubies) {
        cubie.position = rotMatrix * cubie.position;
        cubie.orientation = rotQuat * cubie.orientation;

        cubie.position.x = std::round(cubie.position.x);
        cubie.position.y = std::round(cubie.position.y);
        cubie.position.z = std::round(cubie.position.z);
        cubie.orientation = glm::normalize(cubie.orientation);
    }
    staticCubies.insert(staticCubies.end(), animatedCubies.begin(), animatedCubies.end());
    animatedCubies.clear();

    if (step.recordHistory) {
        stepHistory.push_back(step);
    }
}

void RubiksCubeLogic::undoLogicStep() {
    if (stepHistory.empty()) {
        return; // אין מה לבטל
    }

    // שליפת המהלך האחרון שבוצע
    MoveStep lastStep = stepHistory.back();
    stepHistory.pop_back();

    // ביצוע הפעולה ההפוכה (זווית שלילית) מבלי להוסיף אותה מחדש להיסטוריה
    MoveStep inverseStep = getInverseStep(lastStep);
    applyLogicStep(inverseStep);
}

void RubiksCubeLogic::executeSequence(const std::vector<MoveOption>& sequence, bool animated, bool recordHistory)
{
    for (const auto& moveOption : sequence) {
        MoveStep generatedStep = generateStep(moveOption, animated, recordHistory);
        if (animated) {
            waitingMoves.push(generatedStep);
        } else {
            applyLogicStep(generatedStep);
        }
    }
}

void RubiksCubeLogic::scramble(int stepsCount, bool animated, bool recordHistory)
{
    if (animated && waitingMoves.size() > 0) {
        // אם כבר יש מהלך ממתין, לא מתחילים ערבוב חדש
        return;
    }
    // הגדרת מחולל מספרים אקראיים
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // כל האפשרויות האפשריות לסיבוב
    std::vector<MoveOption> possibleMoves = {
        MoveOption::UP, MoveOption::DOWN,
        MoveOption::RIGHT, MoveOption::LEFT,
        MoveOption::FRONT, MoveOption::BACK
    };
    std::uniform_int_distribution<> moveDist(0, possibleMoves.size() - 1);

    std::vector<MoveOption> scrambleSequence;
    for (int i = 0; i < stepsCount; ++i) {
        MoveOption randomMove = possibleMoves[moveDist(gen)];
        scrambleSequence.push_back(randomMove);
    }
    isScrambling = true; // סימון שהקוביה נמצאת במצב ערבוב

    // הרצת רצף הערבוב
    executeSequence(scrambleSequence, animated, recordHistory);

    if (!animated) {
        isScrambling = false; // אם לא באנימציה, הערבוב הסתיים מיד
    }
}

const std::vector<Cubie>& RubiksCubeLogic::getStaticCubies() const {
    return staticCubies;
}

const std::vector<Cubie>& RubiksCubeLogic::getAnimatedCubies() const {
    return animatedCubies;
}

size_t RubiksCubeLogic::getHistorySize() const {
    return stepHistory.size();
}

bool RubiksCubeLogic::startAnimation(MoveStep step) {
    isAnimating = true;
    if (getIsAnimating()) {
        if (isScrambling) {
            // אם הקוביה נמצאת במצב ערבוב, לא מתחילים אנימציה חדשה
            return false;
        }
        waitingMoves.push(step);
        return false;
    }

    currentAnimation = std::make_unique<AnimationState>();

    currentAnimation->step = step;
    currentAnimation->duration = getDuration(step.move);
    fillAnimatedCubies(step.move);
    return true;
}

bool RubiksCubeLogic::applyMoveAnimated(MoveOption move)
{
    MoveStep step = generateStep(move, true, true);
    return startAnimation(step);
}

bool RubiksCubeLogic::reverseLastMoveAnimated()
{
    // if (isIdle() && stepHistory.empty()) return false; // אין מה לבטל

    // if (!waitingMoves.empty()) {
    //     MoveStep step = waitingMoves.front();
    //     MoveStep reversed = getInverseStep(step);
    //     waitingMoves.pop_front();
    //     return true;
    // }

    // if (getIsAnimating() || !stepHistory.empty()) {
    //     // waitingMoves.clear();
    // }

    // MoveStep inverseMove;
    // if (getIsAnimating() && !getCurrentAnimationStep().waitingToBeReversed) {
    //     currentAnimation->step.waitingToBeReversed = true;
    //     inverseMove = getInverseStep(currentAnimation->step);
        
    // }

    // // שליפת המהלך האחרון שבוצע
    // MoveStep lastMove = stepHistory.back();
    // stepHistory.pop_back();
    // MoveStep inverseMove = getInverseStep(lastMove);
    return false;
}

void RubiksCubeLogic::updateAnimation(float deltaTime, bool *wasAnimating, bool *wasScrambling, bool *wasSolved) {
    *wasAnimating = isAnimating;
    *wasScrambling = isScrambling;
    *wasSolved = isSolved;

    if (getIsAnimating()) {
        currentAnimation->progress += deltaTime;

        if (currentAnimation->progress >= currentAnimation->duration) {
            applyLogicStep(currentAnimation->step);
            currentAnimation.reset(); // מוחק אוטומטית ומשחרר את הזיכרון

            isAnimating = false;
            if (waitingMoves.empty()) {
                isScrambling = false; // אם אין מהלך ממתין, הערבוב הסתיים
            }
            isSolved = checkIsSolved(); // בדיקה אם הקוביה נפתרה לאחר המהלך
        }
    }
    else if (!waitingMoves.empty()) {
        MoveStep nextMove = waitingMoves.front();
        waitingMoves.pop();
        startAnimation(nextMove);
    }
}

bool RubiksCubeLogic::getIsAnimating() const { return currentAnimation != nullptr; }

float arctanEase(float t, float k = 6.0f) {
    // חישוב המונה והמכנה לפי הנוסחה המנורמלת
    float numerator = std::atan(k * (t - 0.5f)) + std::atan(0.5f * k);
    float denominator = 2.0f * std::atan(0.5f * k);
    
    return numerator / denominator;
}
float f(float x) {
    return arctanEase(x, 3.5f);
    // return ((3.0f * x - 7.5f) * x + 5.0f) * x * x * x + 0.5f * x;
    // return (1.0f - std::cos(x * 3.14159265358979323846f)) * 0.5f;
    // return x * x * (3.0f - 2.0f * x);
    // return std::sqrt(x);
}

float RubiksCubeLogic::getAnimationProgress() const
{
    if (!getIsAnimating() || currentAnimation->progress > 1.0f) return 1.0f;
    double progress = currentAnimation->progress / currentAnimation->duration;
    float x = std::min(static_cast<float>(progress), 1.0f);
    return f(x);
}

MoveStep RubiksCubeLogic::getCurrentAnimationStep() const
{
    if (!getIsAnimating()) {
        throw std::runtime_error("No animation is currently running.");
    }
    MoveStep step = currentAnimation->step;
    step.angleRadians *= getAnimationProgress(); // עדכון הזווית לפי התקדמות האנימציה
    return step;
}

glm::mat4 RubiksCubeLogic::getCubieModelMatrix(const Cubie &cubie, bool isAnimated) const
{
    // בדיקה האם הקוביה הזו שייכת לפאה שמסתובבת כרגע
    // נגדיר מראש את הקווטרניון הסופי של הקוביה
    Vector3d finalPosition = cubie.position;
    Quaternion finalOrientation = cubie.orientation;

    if (isAnimated && getIsAnimating()) {
        // בדיקה האם הקוביה הזו נמצאת כרגע בפאה שעוברת אנימציה
        // חישוב זווית האנימציה הנוכחית (למשל מ־0 ועד 90 מעלות לפי התקדמות האנימציה)
        MoveStep currentStep = getCurrentAnimationStep();
        Quaternion animRotation = RubiksCubeLogic::getEntityRotationAxis(currentStep);
        finalPosition = animRotation * cubie.position;
        finalOrientation = animRotation * cubie.orientation;
    }

    // בניית מטריצת ה־Model:
    // א. העברה למיקום הסופי (המעודכן או המקורי)
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, finalPosition);

    // ב. החלת מטריצת הסיבוב מתוך הקווטרניון הסופי
    model = model * glm::toMat4(finalOrientation);
    return model;
}

bool RubiksCubeLogic::checkIsSolved() const {
    // אם הקוביה באמצע אנימציה או שעוד יש מהלכים בתור, היא לא פתורה
    if (!isIdle()) return false;

    for (const auto& cubie : staticCubies) {
        // 1. בדיקת מיקום בגריד
        if (!cubie.isAtHomePosition()) {
            return false;
        }

        // 2. בדיקת האוריינטציה והצבעים דרך המתודה של ה־Cubie
        if (!cubie.isCorrectlyOriented()) {
            return false;
        }
    }
    return true;
}
