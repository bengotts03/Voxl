//
// Created by Ben Gotts on 03/08/2025.
//

#include "Skybox.h"

#include "Texture.h"
#include "VertexBuffer.h"

Skybox::Skybox(Camera* camera) : _camera(camera) {
    _shader = new Shader("../assets/shaders/skybox.vert", "../assets/shaders/skybox.frag");

    Vertex skyboxVertices[] = {
        // Front face
        {{-1.0f, -1.0f,  1.0f}, {1,1,1}, {0,0}, {0,0,-1}},
        {{ 1.0f, -1.0f,  1.0f}, {1,1,1}, {1,0}, {0,0,-1}},
        {{ 1.0f,  1.0f,  1.0f}, {1,1,1}, {1,1}, {0,0,-1}},
        {{-1.0f,  1.0f,  1.0f}, {1,1,1}, {0,1}, {0,0,-1}},

        // Back face
        {{-1.0f, -1.0f, -1.0f}, {1,1,1}, {1,0}, {0,0,1}},
        {{-1.0f,  1.0f, -1.0f}, {1,1,1}, {1,1}, {0,0,1}},
        {{ 1.0f,  1.0f, -1.0f}, {1,1,1}, {0,1}, {0,0,1}},
        {{ 1.0f, -1.0f, -1.0f}, {1,1,1}, {0,0}, {0,0,1}},

        // Left face
        {{-1.0f, -1.0f, -1.0f}, {1,1,1}, {0,0}, {-1,0,0}},
        {{-1.0f, -1.0f,  1.0f}, {1,1,1}, {1,0}, {-1,0,0}},
        {{-1.0f,  1.0f,  1.0f}, {1,1,1}, {1,1}, {-1,0,0}},
        {{-1.0f,  1.0f, -1.0f}, {1,1,1}, {0,1}, {-1,0,0}},

        // Right face
        {{ 1.0f, -1.0f, -1.0f}, {1,1,1}, {1,0}, {1,0,0}},
        {{ 1.0f,  1.0f, -1.0f}, {1,1,1}, {1,1}, {1,0,0}},
        {{ 1.0f,  1.0f,  1.0f}, {1,1,1}, {0,1}, {1,0,0}},
        {{ 1.0f, -1.0f,  1.0f}, {1,1,1}, {0,0}, {1,0,0}},

        // Top face
        {{-1.0f,  1.0f, -1.0f}, {1,1,1}, {0,1}, {0,1,0}},
        {{-1.0f,  1.0f,  1.0f}, {1,1,1}, {0,0}, {0,1,0}},
        {{ 1.0f,  1.0f,  1.0f}, {1,1,1}, {1,0}, {0,1,0}},
        {{ 1.0f,  1.0f, -1.0f}, {1,1,1}, {1,1}, {0,1,0}},

        // Bottom face
        {{-1.0f, -1.0f, -1.0f}, {1,1,1}, {1,1}, {0,-1,0}},
        {{ 1.0f, -1.0f, -1.0f}, {1,1,1}, {0,1}, {0,-1,0}},
        {{ 1.0f, -1.0f,  1.0f}, {1,1,1}, {0,0}, {0,-1,0}},
        {{-1.0f, -1.0f,  1.0f}, {1,1,1}, {1,0}, {0,-1,0}}
    };
    unsigned int skyboxIndices[] = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20
    };

    std::vector<Vertex> skyboxVerts(skyboxVertices, skyboxVertices + sizeof(skyboxVertices) / sizeof(Vertex));
    std::vector<GLuint> skyboxInds(skyboxIndices, skyboxIndices + sizeof(skyboxIndices) / sizeof(GLuint));
    std::vector<Texture> skyboxTex;

    _mesh = new Mesh(skyboxVerts, skyboxInds, skyboxTex);
}

Skybox::~Skybox() {
    _shader->Delete();
    delete _mesh;
}

void Skybox::Render() {
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_FRONT);

    _shader->Activate();
    _mesh->DrawViewProj(*_shader, *_camera);

    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);
}
