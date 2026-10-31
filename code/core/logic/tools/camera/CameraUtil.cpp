#include "CameraUtil.hpp"
#include <algorithm>
#include <cmath>

glm::mat4 CameraUtil::getViewMatrix() const {
    // המרה מקואורדינטות כדוריות (Spherical Coordinates) לקרטזיות (XYZ)
    float radYaw = glm::radians(cameraYaw);
    float radPitch = glm::radians(cameraPitch);

    glm::vec3 position;
    position.x = cameraDistance * cos(radPitch) * sin(radYaw);
    position.y = cameraDistance * sin(radPitch);
    position.z = cameraDistance * cos(radPitch) * cos(radYaw);

    // מיקום המצלמה הוא ביחס לנקודת המטרה (המרכז)
    position += target;

    // ציר ה-Y מוגדר כמעלה (Up vector)
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    return glm::lookAt(position, target, up);
}

void CameraUtil::processMouseMovement(float xoffset, float yoffset, float sensitivity) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cameraYaw -= xoffset;
    cameraPitch -= yoffset;

    // הגבלת ה-Pitch כדי שהמצלמה לא תעשה "פליפ" מלא מעל הקוטב
    if (cameraPitch > 89.0f)  cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;
}

void CameraUtil::processMouseScroll(float yoffset, float zoomSpeed) {
    cameraDistance -= yoffset * zoomSpeed;

    // הגבלת מרחק מינימלי ומקסימלי כדי שהמצלמה לא תיכנס בתוך הקוביה או תתרחק מדי
    if (cameraDistance < 2.0f)  cameraDistance = 2.0f;
    if (cameraDistance > 15.0f) cameraDistance = 15.0f;
}
