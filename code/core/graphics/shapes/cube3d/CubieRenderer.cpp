#include "CubieRenderer.hpp"

CubieRenderer::CubieRenderer() {
    initBuffers();
}

CubieRenderer::~CubieRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

Vector3d CubieRenderer::getColorRgb(ColorEnum color) const {
    switch (color) {
        case ColorEnum::WHITE:  return Vector3d(1.0f, 1.0f, 1.0f);
        case ColorEnum::YELLOW: return Vector3d(1.0f, 1.0f, 0.0f);
        case ColorEnum::RED:    return Vector3d(1.0f, 0.0f, 0.0f);
        case ColorEnum::ORANGE: return Vector3d(1.0f, 0.5f, 0.0f);
        case ColorEnum::BLUE:   return Vector3d(0.0f, 0.0f, 1.0f);
        case ColorEnum::GREEN:  return Vector3d(0.0f, 1.0f, 0.0f);
        case ColorEnum::NONE:   
        default:            return Vector3d(0.15f, 0.15f, 0.15f); // אפור כהה לפאות פנימיות
    }
}

void CubieRenderer::initBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // מקצים מקום ל-36 ורטיקסים (6 פאות * 6 קודקודים למשולשים), כל ורטיקס מכיל 3 מיקום + 3 צבע
    glBufferData(GL_ARRAY_BUFFER, 36 * 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CubieRenderer::draw(const Cubie& cubie) const {
    // שליפת צבעי הפאות מהלוגיקה והמרתם ל־RGB
    Vector3d topColor    = getColorRgb(cubie.getColor(FaceIndex::FACE_TOP));
    Vector3d bottomColor = getColorRgb(cubie.getColor(FaceIndex::FACE_BOTTOM));
    Vector3d rightColor  = getColorRgb(cubie.getColor(FaceIndex::FACE_RIGHT));
    Vector3d leftColor   = getColorRgb(cubie.getColor(FaceIndex::FACE_LEFT));
    Vector3d frontColor  = getColorRgb(cubie.getColor(FaceIndex::FACE_FRONT));
    Vector3d backColor   = getColorRgb(cubie.getColor(FaceIndex::FACE_BACK));

    float size = 0.45f; // רווח קטן בין הקוביות

    // 36 ורטיקסים (מיקום x,y,z + צבע r,g,b לכל קודקוד)
    float vertices[] = {
        // Top Face
        -size, -size,  size,  topColor.r, topColor.g, topColor.b,
        size, -size,  size,  topColor.r, topColor.g, topColor.b,
        size,  size,  size,  topColor.r, topColor.g, topColor.b,
        size,  size,  size,  topColor.r, topColor.g, topColor.b,
        -size,  size,  size,  topColor.r, topColor.g, topColor.b,
        -size, -size,  size,  topColor.r, topColor.g, topColor.b,

        // Bottom Face (2 triangles)
        -size, -size, -size,  bottomColor.r, bottomColor.g, bottomColor.b,
         size, -size, -size,  bottomColor.r, bottomColor.g, bottomColor.b,
         size,  size, -size,  bottomColor.r, bottomColor.g, bottomColor.b,
         size,  size, -size,  bottomColor.r, bottomColor.g, bottomColor.b,
        -size,  size, -size,  bottomColor.r, bottomColor.g, bottomColor.b,
        -size, -size, -size,  bottomColor.r, bottomColor.g, bottomColor.b,

        // Left Face
        -size, -size, -size,  leftColor.r, leftColor.g, leftColor.b,
        -size, -size,  size,  leftColor.r, leftColor.g, leftColor.b,
        -size,  size,  size,  leftColor.r, leftColor.g, leftColor.b,
        -size,  size,  size,  leftColor.r, leftColor.g, leftColor.b,
        -size,  size, -size,  leftColor.r, leftColor.g, leftColor.b,
        -size, -size, -size,  leftColor.r, leftColor.g, leftColor.b,

        // Right Face
         size, -size, -size,  rightColor.r, rightColor.g, rightColor.b,
         size, -size,  size,  rightColor.r, rightColor.g, rightColor.b,
         size,  size,  size,  rightColor.r, rightColor.g, rightColor.b,
         size,  size,  size,  rightColor.r, rightColor.g, rightColor.b,
         size,  size, -size,  rightColor.r, rightColor.g, rightColor.b,
         size, -size, -size,  rightColor.r, rightColor.g, rightColor.b,

        // Front Face
        -size,  size, -size,  frontColor.r, frontColor.g, frontColor.b,
         size,  size, -size,  frontColor.r, frontColor.g, frontColor.b,
         size,  size,  size,  frontColor.r, frontColor.g, frontColor.b,
         size,  size,  size,  frontColor.r, frontColor.g, frontColor.b,
        -size,  size,  size,  frontColor.r, frontColor.g, frontColor.b,
        -size,  size, -size,  frontColor.r, frontColor.g, frontColor.b,

        // Back Face
        -size, -size, -size,  backColor.r, backColor.g, backColor.b,
         size, -size, -size,  backColor.r, backColor.g, backColor.b,
         size, -size,  size,  backColor.r, backColor.g, backColor.b,
         size, -size,  size,  backColor.r, backColor.g, backColor.b,
        -size, -size,  size,  backColor.r, backColor.g, backColor.b,
        -size, -size, -size,  backColor.r, backColor.g, backColor.b
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
