//
// Created by Ben Gotts on 06/08/2025.
//

#include "Gizmos.h"

#include <spdlog/spdlog.h>

Shader* Gizmos::_gizmosShader = nullptr;
Mesh* Gizmos::_gizmosMesh = nullptr;
Camera* Gizmos::_camera = nullptr;

std::vector<Vertex> Gizmos::_vertices;
std::vector<GLuint> Gizmos::_indices;
std::vector<Texture> Gizmos::_textures;

Gizmos::Gizmos(Camera* cam) {
    _gizmosShader = new Shader("../assets/shaders/gizmos.vert", "../assets/shaders/gizmos.frag");
    _camera = cam;

    _gizmosMesh = new Mesh(_vertices, _indices, _textures);
    _gizmosMesh->SetCustomDrawMode(GL_LINES);

    _textures.emplace_back("../assets/textures/WhitePixel.png", "diffuse", 0);
}

Gizmos::~Gizmos() {
    if (_gizmosMesh)
        delete _gizmosMesh;
}

void Gizmos::StartRender() {
    _vertices.clear();
    _indices.clear();
}

void Gizmos::EndRender() {
    _gizmosMesh = new Mesh(_vertices, _indices, _textures);

    Flush();
}

void Gizmos::Flush() {
    glLineWidth(20.0f);

    _gizmosShader->Activate();
    _gizmosMesh->Draw(*_gizmosShader, *_camera, Together);

    // glLineWidth(1.0f);
}

void Gizmos::DrawLine(glm::vec3 startPos, glm::vec3 endPos) {
    GLuint startIndex = _vertices.size();

    _vertices.push_back({{startPos}, {0, 0.35f, 1}, {}, {}});
    _vertices.push_back({{endPos}, {0, 0.35f, 1}, {}, {}});

    _indices.push_back(startIndex);
    _indices.push_back(startIndex + 1);
}

void Gizmos::DrawWireCube(glm::vec3 centrePos, float size) {
}
