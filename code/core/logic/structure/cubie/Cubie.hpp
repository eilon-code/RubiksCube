#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "MathTypes.hpp"

class Cubie {
public:
    // בנאי שמקבל את המיקום ההתחלתי
    Cubie(const Vector3d& startPos, const Quaternion& startOrientation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f));

    // מתודת ממשק ציבורית לקבלת צבע של פאה מסוימת
    ColorEnum getColor(FaceIndex face) const;
    bool isCorrectlyOriented() const;
    bool isAtHomePosition() const;

    // מתודות נוספות לניהול מיקום וסיבוב (במידת הצורך)
    Vector3d homePosition; // מיקום "בית" של הקוביה, לצורך בדיקת פתור
    Vector3d position;
    Quaternion orientation;

private:
    ColorEnum faceColors[6];
};
