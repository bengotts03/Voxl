//
// Created by Ben Gotts on 03/08/2025.
//

#ifndef SKYBOX_H
#define SKYBOX_H
#include "../Rendering/Mesh.h"
#include "../Rendering/Shader.h"

class Skybox {
public:
    Skybox(Camera* cam);
    ~Skybox();

    void Render();
private:
    Shader* _shader;
    Mesh* _mesh;
    Camera* _camera;
};

#endif //SKYBOX_H