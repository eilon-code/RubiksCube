#ifndef MATH_TYPES_HPP
#define MATH_TYPES_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
// הגדרת כינויים (Aliases) לנוחות וקריאות
using Vector3d = glm::vec3;
using Quaternion = glm::quat;

enum ColorEnum {
    NONE = -1,
    WHITE, YELLOW, RED, ORANGE, GREEN, BLUE,
};

enum FaceIndex {
    FACE_TOP = ColorEnum::WHITE,
    FACE_BOTTOM = ColorEnum::YELLOW,
    FACE_RIGHT = ColorEnum::RED,
    FACE_LEFT = ColorEnum::ORANGE,
    FACE_FRONT = ColorEnum::GREEN,
    FACE_BACK = ColorEnum::BLUE,
};

// מערך קבוע של הנורמלים המקומיים, מותאם בדיוק לסדר של FaceIndex
const glm::vec3 LOCAL_NORMALS[6] = {
    { 0.0f,  0.0f,  1.0f}, // FACE_TOP (Z+)
    { 0.0f,  0.0f, -1.0f}, // FACE_BOTTOM (Z-)
    { 1.0f,  0.0f,  0.0f}, // FACE_RIGHT (+X)
    {-1.0f,  0.0f,  0.0f}, // FACE_LEFT  (-X)
    { 0.0f,  1.0f,  0.0f}, // FACE_FRONT (Y+)
    { 0.0f, -1.0f,  0.0f}  // FACE_BACK  (Y-)
};

enum class MoveOption {
    NONE = -1,
    UP,     // +Z
    DOWN,   // -Z
    RIGHT,  // +X
    LEFT,   // -X
    FRONT,  // +Y
    BACK    // -Y
};

#endif // MATH_TYPES_HPP
