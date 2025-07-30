//
// Created by Ben Gotts on 27/07/2025.
//

#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <glm/glm.hpp>

struct BoundingBox {
    glm::vec3 MinBounds;
    glm::vec3 MaxBounds;

    glm::vec3 Centre;
    glm::vec3 Extents;

    BoundingBox() : MinBounds(glm::vec3(0)), MaxBounds(glm::vec3(0)) {
        Init();
    }
    BoundingBox(glm::vec3 minBounds, glm::vec3 maxBounds) : MinBounds(minBounds), MaxBounds(maxBounds) {
        Init();
    }

    void Init() {
        Centre = {(MinBounds + MaxBounds) * 0.5f};
        Extents = {MaxBounds.x - Centre.x, MaxBounds.y - Centre.y, MaxBounds.z - Centre.z};
    }
};

#endif //BOUNDINGBOX_H
