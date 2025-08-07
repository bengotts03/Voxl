//
// Created by Ben Gotts on 03/08/2025.
//

#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "Mesh.h"
#include "Shader.h"

class Crosshair {
public:
    Crosshair(Camera* cam);
    ~Crosshair();

    void Render();
private:
    Shader* _shader;
    Mesh* _mesh;
    Camera* _camera;
};

#endif //CROSSHAIR_H
