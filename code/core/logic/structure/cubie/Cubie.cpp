#include "Cubie.hpp"

Cubie::Cubie(const Vector3d& startPos, const Quaternion& startOrientation) 
    : homePosition(startPos), position(startPos), orientation(startOrientation) {
    // אתחול ראשוני של כל הפאות ל־NONE (או צבעים ברירת מחדל)
    for (int i = 0; i < 6; ++i) {
        faceColors[i] = ColorEnum::NONE;
    }

    if (startPos.z ==  1.0f) faceColors[FACE_TOP]    = ColorEnum::WHITE;
    if (startPos.z == -1.0f) faceColors[FACE_BOTTOM] = ColorEnum::YELLOW;
    if (startPos.x ==  1.0f) faceColors[FACE_RIGHT]  = ColorEnum::RED;
    if (startPos.x == -1.0f) faceColors[FACE_LEFT]   = ColorEnum::ORANGE;
    if (startPos.y ==  1.0f) faceColors[FACE_FRONT]  = ColorEnum::GREEN;
    if (startPos.y == -1.0f) faceColors[FACE_BACK]   = ColorEnum::BLUE;
}

ColorEnum Cubie::getColor(FaceIndex face) const
{
    // ממירים את ה־Enum של הפאה לאינדקס מספרי במערך (0 עד 5)
    return faceColors[static_cast<int>(face)];
}

bool Cubie::isCorrectlyOriented() const {
    for (int i = 0; i < 6; ++i) {
        ColorEnum color = faceColors[i];
        if (color == ColorEnum::NONE) continue; // פאה פנימית שלא צריכה בדיקה

        // חישוב הכיוון הגלובלי של הפאה
        glm::vec3 globalNormal = orientation * LOCAL_NORMALS[i];

        // מכפלה סקלרית ישירות מול וקטור הנורמל המקומי המקורי
        float dotProduct = glm::dot(globalNormal, LOCAL_NORMALS[i]);

        // אם המכפלה הסקלרית קטנה מ-1 (בסבילות קטנה לזליגות נקודה צפה), הפאה אינה במקומה
        if (dotProduct < 0.99f) {
            return false;
        }
    }

    return true;
}

bool Cubie::isAtHomePosition() const
{
    return glm::distance(position, homePosition) < 0.05f;
}
