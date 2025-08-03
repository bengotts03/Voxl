//
// Created by Ben Gotts on 03/08/2025.
//

#ifndef PHYSICS_H
#define PHYSICS_H
#include "src/Voxl/VoxelWorld.h"

struct RaycastHit {
    WorldPosition WorldPositionHit{};
    ChunkPosition ChunkPositionHit{};
    VoxelPosition VoxelPositionHit{};
    LocalVoxelPosition LocalVoxelPositionHit{};

    glm::vec3 Direction{};

    glm::vec3 HitPoint{};
    glm::vec3 HitNormal{};
};

class VoxelWorld;
class Physics {
public:
    explicit Physics(VoxelWorld* world);
    ~Physics();

    static bool Raycast(glm::vec3 origin, glm::vec3 direction, RaycastHit& hit);
    static bool Raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, RaycastHit& hit);
private:
    static void CalculateHitNormal(RaycastHit& hit);
private:
    static VoxelWorld* _worldContext;
};

#endif //PHYSICS_H
