//
// Created by Ben Gotts on 06/08/2025.
//

#ifndef VOXL_GIZMOS_H
#define VOXL_GIZMOS_H

#include <glm/vec3.hpp>

#include "src/Rendering/Mesh.h"

#define BATCH_VERT_COUNT (int)1000

class Gizmos {
public:
    Gizmos(Camera* cam);
    ~Gizmos();

    static void StartRender();
    static void EndRender();
    static void Flush();

    static void DrawLine(glm::vec3 startPos, glm::vec3 endPos);
    static void DrawWireCube(glm::vec3 centrePos, float size);
private:
    static Shader* _gizmosShader;
    static Camera* _camera;
    static Mesh* _gizmosMesh;

    static std::vector<Vertex> _vertices;
    static std::vector<GLuint> _indices;
    static std::vector<Texture> _textures;
};

#endif //VOXL_GIZMOS_H