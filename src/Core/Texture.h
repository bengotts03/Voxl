//
// Created by Ben Gotts on 05/07/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"
#include "stb_image.h"
#include <string>

#include "Shader.h"

class Texture {
public:
    GLuint ID;
    const char* Type;

    Texture(std::string texturePath, const char* type, GLuint slot);
    ~Texture() = default;

    void Bind();
    void Unbind();
    void Delete();
    void TextureUnit(Shader& shader, const char* uniform, GLuint slot);
private:
    int _textureWidth;
    int _textureHeight;
    int _numberOfColourChannels;
    int _textureSlot;
};

#endif //TEXTURE_H
