//
// Created by Ben Gotts on 03/08/2025.
//

#include "Crosshair.h"

Crosshair::Crosshair(Camera* camera) : _camera(camera){
    _shader = new Shader("../assets/shaders/crosshair.vert", "../assets/shaders/crosshair.frag");

    Vertex crosshairQuad[] = {
        {{-1.0f, -1.0f,  1.0f}, {1,1,1}, {0,0}, {0,0,-1}},
        {{ 1.0f, -1.0f,  1.0f}, {1,1,1}, {1,0}, {0,0,-1}},
        {{ 1.0f,  1.0f,  1.0f}, {1,1,1}, {1,1}, {0,0,-1}},
        {{-1.0f,  1.0f,  1.0f}, {1,1,1}, {0,1}, {0,0,-1}}
    };
    unsigned int crosshairIndices[] = {
        0, 1, 2, 2, 3, 0
    };

    std::vector<Vertex> crosshairVerts(crosshairQuad, crosshairQuad + sizeof(crosshairQuad) / sizeof(Vertex));
    std::vector<GLuint> crosshairInds(crosshairIndices, crosshairIndices + sizeof(crosshairIndices) / sizeof(GLuint));
    std::vector<Texture> crosshairTex;
    crosshairTex.emplace_back("../assets/textures/Crosshair.png", "diffuse", 0);

    // _mesh = new Mesh(crosshairVerts, crosshairInds, crosshairTex);
}

Crosshair::~Crosshair() {
    _shader->Delete();
    delete _mesh;
}

void Crosshair::Render() {
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_FRONT);

    _shader->Activate();
    _mesh->Draw(*_shader, *_camera, Separate);

    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);
}
