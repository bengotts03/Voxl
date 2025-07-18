//
// Created by Ben Gotts on 05/07/2025.
//

#include "VertexArray.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &ID);
}

/**
 * \brief Links a vertex buffer attribute to the vertex array.
 *
 * Sets up the vertex attribute pointer and enables the attribute for the given layout.
 *
 * \param vertexBuffer The vertex buffer to bind.
 * \param layout The index of the vertex attribute (location in the shader).
 * \param numberOfComponents Number of components per attribute (e.g., 2 for vec2).
 * \param type Data type of each component (e.g., GL_FLOAT).
 * \param stride Offset between each vertex
 * \param offset Offset of the first component in the buffer. (e.g. where this component starts in vertex memory)
 */
void VertexArray::LinkAttribute(VertexBuffer& vertexBuffer, GLuint layout, GLuint numberOfComponents, GLenum type, GLsizeiptr stride, void* offset){
    vertexBuffer.Bind();

    glVertexAttribPointer(layout, numberOfComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);

    vertexBuffer.Unbind();
}

void VertexArray::Bind() {
    glBindVertexArray(ID);
}

void VertexArray::Unbind() {
    glBindVertexArray(0);
}

void VertexArray::Delete() {
    glDeleteBuffers(1, &ID);
}

