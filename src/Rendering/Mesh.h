//
// Created by Ben Gotts on 06/07/2025.
//

#ifndef MESH_H
#define MESH_H

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "../Rendering/Texture.h"
#include "../Core/Camera.h"
#include "VertexBuffer.h"

enum ProjectionMethod {
    Together,
    Separate
};

class Mesh {
public:
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

    void Draw(Shader& shader, Camera& camera, ProjectionMethod projectionMethod);

    int GetNumVerts() const;
    int GetNumIndices() const;

    void SetCustomDrawMode(int drawMode);

    glm::vec3 Position = glm::vec3(0, 0, 0);
private:
    std::vector<Vertex> _vertices;
    std::vector<GLuint> _indices;
    std::vector<Texture> _textures;

    VertexArray _vertexArrayObject;

    int _drawMode = GL_TRIANGLES;
};

#endif //MESH_H
