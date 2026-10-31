#pragma once

#include <GL/glew.h>

class TriangleRenderer {
public:
    TriangleRenderer();
    ~TriangleRenderer();

    void draw() const;

private:
    GLuint VAO, VBO;
    void initBuffers();
};
