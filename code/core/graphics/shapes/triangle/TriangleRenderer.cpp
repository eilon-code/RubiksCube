#include "TriangleRenderer.hpp"

TriangleRenderer::TriangleRenderer() {
    initBuffers();
}

TriangleRenderer::~TriangleRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void TriangleRenderer::initBuffers() {
    float vertices[] = {
         0.0f,  0.5f, 0.0f,  // קודקוד עליון
        -0.5f, -0.5f, 0.0f,  // קודקוד שמאלי תחתון
         0.5f, -0.5f, 0.0f   // קודקוד ימני תחתון
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TriangleRenderer::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
