//
// Created by Ben Gotts on 05/07/2025.
//

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <glad/glad.h>
#include "VertexBuffer.h"

class VertexArray {
public:
    VertexArray();
    ~VertexArray() = default;

    void LinkAttribute(VertexBuffer& vertexBuffer, GLuint layout, GLuint numberOfComponents, GLenum type, GLsizeiptr stride, void* offset);
    void Bind();
    void Unbind();
    void Delete();
private:
    GLuint ID;
};

#endif //VERTEXARRAY_H
