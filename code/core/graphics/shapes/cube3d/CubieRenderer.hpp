#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "MathTypes.hpp"
#include "Cubie.hpp"

class CubieRenderer {
public:
    CubieRenderer();
    ~CubieRenderer();

    void draw(const Cubie& cubie) const;

private:
    GLuint VAO, VBO;
    void initBuffers();
    Vector3d getColorRgb(ColorEnum color) const;
};
