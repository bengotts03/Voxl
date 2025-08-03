//
// Created by Ben Gotts on 03/08/2025.
//

#include "Physics.h"

VoxelWorld* Physics::_worldContext;

Physics::Physics(VoxelWorld* world) {
    _worldContext = world;
}

Physics::~Physics() {
}

bool Physics::Raycast(glm::vec3 origin, glm::vec3 direction, RaycastHit& hit) {
    float maxDistance = 100.0f;

    return Raycast(origin, direction, maxDistance, hit);
}

bool Physics::Raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, RaycastHit& hit) {
    glm::vec3 normalizedDir = glm::normalize(direction);
    float stepSize = VOXEL_SIZE * 0.1f;

    for (float i = 0; i < maxDistance; i += stepSize) {
        glm::vec3 currentPos = origin + normalizedDir * i;

        WorldPosition worldPos = {currentPos};

        auto chunk = _worldContext->GetChunk(worldPos);
        if (chunk == nullptr) {
            continue;
        }

        LocalVoxelPosition localVoxelPos = WorldPositionToLocalVoxel(worldPos);
        glm::ivec3 pos = localVoxelPos.Position;

        if (pos.x >= 0 && pos.y >= 0 && pos.z >= 0 &&
            pos.x < CHUNK_SIZE && pos.y < CHUNK_SIZE && pos.z < CHUNK_SIZE) {
            auto voxel = chunk->GetVoxelBlock(localVoxelPos);

            if (voxel && voxel->IsActive()) {
                ChunkPosition chunkPos = WorldPositionToChunk(worldPos);
                VoxelPosition voxelPos = WorldPositionToVoxel(worldPos);

                glm::vec3 voxelCenter = glm::vec3(pos) * VOXEL_SIZE + chunk->GetWorldPosition().Position;
                glm::vec3 localHitPoint = (currentPos - voxelCenter) / VOXEL_SIZE;

                hit = RaycastHit(
                    worldPos, chunkPos, voxelPos, localVoxelPos,
                    direction,
                    localHitPoint);

                CalculateHitNormal(hit);

                return true;
            }
        }
    }

    return false;
}

void Physics::CalculateHitNormal(RaycastHit& hit) {
    glm::vec3 localHit = hit.HitPoint;
    glm::vec3 absHit = glm::abs(localHit);

    if (absHit.y >= absHit.x && absHit.y >= absHit.z) {
        if (localHit.y > 0) {
            hit.HitNormal = glm::vec3(0.0f, 1.0f, 0.0f);
        } else {
            hit.HitNormal = glm::vec3(0.0f, -1.0f, 0.0f);
        }
    }
    else if (absHit.x >= absHit.z) {
        if (localHit.x > 0) {
            hit.HitNormal = glm::vec3(1.0f, 0.0f, 0.0f);
        } else {
            hit.HitNormal = glm::vec3(-1.0f, 0.0f, 0.0f);
        }
    }
    else {
        if (localHit.z > 0) {
            hit.HitNormal = glm::vec3(0.0f, 0.0f, 1.0f);
        } else {
            hit.HitNormal = glm::vec3(0.0f, 0.0f, -1.0f);
        }
    }
}
