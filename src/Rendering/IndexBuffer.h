//
// Created by Ben Gotts on 05/07/2025.
//

#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <vector>
#include <glad/glad.h>

class IndexBuffer {
public:
    IndexBuffer(std::vector<GLuint>& indices);
    ~IndexBuffer() = default;

    void Bind();
    void Unbind();
    void Delete();
private:
    GLuint ID;
};



#endif //INDEXBUFFER_H
