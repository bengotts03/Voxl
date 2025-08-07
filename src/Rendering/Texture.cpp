//
// Created by Ben Gotts on 05/07/2025.
//

#include "Texture.h"

Texture::Texture(std::string texturePath, const char* type, GLuint slot) : Type(type), _textureSlot(slot) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes;

    bytes = stbi_load(texturePath.c_str(), &_textureWidth, &_textureHeight,
            &_numberOfColourChannels, 0);

    GLenum format = GL_RGB;
    if (_numberOfColourChannels == 4) {
        format = GL_RGBA;
    }

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, format, _textureWidth, _textureHeight, 0, format, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
}

void Texture::TextureUnit(Shader& shader, const char* uniform, GLuint slot) {
    shader.SetUniform1i(uniform, slot);
}

void Texture::Bind() {
    glActiveTexture(GL_TEXTURE0 + _textureSlot);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
    glDeleteTextures(1, &ID);
}