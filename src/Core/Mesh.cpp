//
// Created by Ben Gotts on 06/07/2025.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
: _vertices(vertices), _indices(indices), _textures(textures)
{
    _vertexArrayObject.Bind();

    auto vertexBufferObject = VertexBuffer(_vertices);
    auto indexBufferObject = IndexBuffer(_indices);

    // Link vertex position as attribute layout 0
    _vertexArrayObject.LinkAttribute(vertexBufferObject, 0, 3,
        GL_FLOAT, 11 * sizeof(float), (void*)0);
    // Link vertex colour as attribute layout 1
    _vertexArrayObject.LinkAttribute(vertexBufferObject, 1, 3,
        GL_FLOAT, 11 * sizeof(float), (void*)(sizeof(float) * 3));
    // Link vertex tex coords as attribute layout 2
    _vertexArrayObject.LinkAttribute(vertexBufferObject, 2, 2,
        GL_FLOAT, 11 * sizeof(float), (void*)(sizeof(float) * 6));
    // Link vertex normals as attribute layout 3
    _vertexArrayObject.LinkAttribute(vertexBufferObject, 3, 3,
        GL_FLOAT, 11 * sizeof(float), (void*)(sizeof(float) * 8));

    _vertexArrayObject.Unbind();
    vertexBufferObject.Unbind();
    indexBufferObject.Unbind();
}

void Mesh::Draw(Shader &shader, Camera &camera)
{
    shader.Activate();
    _vertexArrayObject.Bind();

    unsigned int numberOfDiffuse = 0;
    unsigned int numberOfSpecular = 0;

    for (int i = 0; i < _textures.size(); ++i) {
        std::string num;
        std::string type = _textures[i].Type;

        if (type == "diffuse") {
            num = std::to_string(numberOfDiffuse++);
        }
        else if (type == "specular") {
            num = std::to_string(numberOfSpecular++);
        }

        _textures[i].TextureUnit(shader, (type + num).c_str(), i);
        _textures[i].Bind();
    }

    camera.CalculateMatrix(shader, "cameraMatrix");

    auto model = glm::mat4(1);
    model = glm::translate(model, Position);

    shader.SetUniformMatrix4("model", model);

    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::DrawViewProj(Shader &shader, Camera &camera) {
    shader.Activate();
    _vertexArrayObject.Bind();

    unsigned int numberOfDiffuse = 0;
    unsigned int numberOfSpecular = 0;

    for (int i = 0; i < _textures.size(); ++i) {
        std::string num;
        std::string type = _textures[i].Type;

        if (type == "diffuse") {
            num = std::to_string(numberOfDiffuse++);
        }
        else if (type == "specular") {
            num = std::to_string(numberOfSpecular++);
        }

        _textures[i].TextureUnit(shader, (type + num).c_str(), i);
        _textures[i].Bind();
    }

    shader.SetUniformMatrix4("view", camera.GetView());
    shader.SetUniformMatrix4("projection", camera.GetProjection());

    auto model = glm::mat4(1);
    model = glm::translate(model, Position);

    shader.SetUniformMatrix4("model", model);

    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
}