#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraUtil {
public:
    // נתוני המצלמה המבוקשים
    float cameraYaw = 0.0f;       // סיבוב אופקי (במעלות)
    float cameraPitch = 20.0f;    // סיבוב אנכי (התחלתי קצת מלמעלה לנוחות)
    float cameraDistance = 6.0f;  // מרחק המצלמה מהמרכז

    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f); // הנקודה שהמצלמה מסתכלת עליה (מרכז הקוביה)

    CameraUtil() = default;

    // מחזיר את מטריצת ה-View עבור OpenGL בהתאם למיקום המצלמה הנוכחי
    glm::mat4 getViewMatrix() const;

    // עדכון המצלמה בעקבות תזוזת עכבר (גרירה)
    void processMouseMovement(float xoffset, float yoffset, float sensitivity = 0.3f);

    // עדכון המרחק בעקבות גלילת עכבר (Zoom In/Out)
    void processMouseScroll(float yoffset, float zoomSpeed = 0.5f);
};

#endif // CAMERA_HPP
