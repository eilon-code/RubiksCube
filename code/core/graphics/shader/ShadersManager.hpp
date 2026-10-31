#pragma once

#include <string>
#include <GL/glew.h> // או ספרית ה-OpenGL בה אתה משתמש
#include <glm/glm.hpp>

class ShadersManager {
public:
    GLuint programID; // מזהה תוכנית השיידרים ב-GPU

    // בנאי המקבל את נתיבי הקבצים ומבצע טעינה וקימפול אוטומטיים
    ShadersManager(const std::string& vertexPath, const std::string& fragmentPath);
    ~ShadersManager();

    // הפעלת השיידר לשימוש ברינדור
    void use() const;

    // מחלקות עזר להעברת משתנים (Uniforms) לשיידר בקלות בעזרת GLM
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;

private:
    // פונקציות עזר פנימיות לקימפול
    GLuint compileShader(GLenum type, const std::string& source);
    std::string loadShaderSource(const std::string& filepath);
};
