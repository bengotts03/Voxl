//
// Created by Ben Gotts on 06/07/2025.
//

#ifndef MESH_H
#define MESH_H

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Camera.h"
#include "VertexBuffer.h"

class Mesh {
public:
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

    void Draw(Shader& shader, Camera& camera);
    void DrawViewProj(Shader& shader, Camera& camera);

    glm::vec3 Position = glm::vec3(0, 0, 0);
private:
    std::vector<Vertex> _vertices;
    std::vector<GLuint> _indices;
    std::vector<Texture> _textures;

    VertexArray _vertexArrayObject;
};

#endif //MESH_H
