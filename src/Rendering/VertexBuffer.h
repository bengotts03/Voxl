//
// Created by Ben Gotts on 05/07/2025.
//

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 colour;
    glm::vec2 textureCoords;
    glm::vec3 normals;
};

class VertexBuffer {
public:
    VertexBuffer(std::vector<Vertex>& vertices);
    ~VertexBuffer() = default;

    void Bind();
    void Unbind();
    void Delete();
private:
    GLuint ID;
};

#endif //VERTEXBUFFER_H