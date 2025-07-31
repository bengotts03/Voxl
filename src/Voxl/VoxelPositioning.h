//
// Created by Ben Gotts on 17/07/2025.
//

#ifndef VOXELPOSITIONING_H
#define VOXELPOSITIONING_H

#include <glm/glm.hpp>

#define CHUNK_SIZE (int)32
#define VOXEL_SIZE (float)0.1f

/**
 * @brief Represents a position in global world space.
 *
 * This is a floating-point vector that represents the exact location
 * of an entity or object in the continuous world. Used for rendering,
 * physics, and movement calculations.
 */
struct WorldPosition {
    glm::vec3 Position;

    WorldPosition() : Position(0, 0, 0){}
    WorldPosition(glm::vec3 pos) : Position(pos){}
    WorldPosition(float x, float y, float z) : Position(glm::vec3(x, y, z)){}
};

/**
 * @brief Represents the world-aligned position of a chunk.
 *
 * This is typically a multiple of CHUNK_SIZE in each axis. While stored
 * as a float for compatibility with rendering, this position corresponds
 * to discrete chunk indices scaled into world space.
 */
struct ChunkPosition {
    glm::ivec3 Position;

    ChunkPosition() : Position(0, 0, 0){}
    ChunkPosition(glm::ivec3 pos) : Position(pos){}
    ChunkPosition(int x, int y, int z) : Position(glm::ivec3(x, y, z)){}
};

/**
 * @brief Represents the world-space position of an individual voxel.
 *
 * Can be used to track which voxel a ray hits, or to convert from world
 * position into a voxel grid coordinate. Typically snapped to voxel size.
 */
struct VoxelPosition {
    glm::ivec3 Position;
};

/**
 * @brief Position of a voxel relative to its parent chunk.
 *
 * This represents a voxel's local coordinate inside a chunk. Should be
 * clamped between 0 and CHUNK_SIZE - 1 on each axis.
 */
struct LocalVoxelPosition {
    glm::ivec3 Position;
};

static ChunkPosition WorldPositionToChunk(WorldPosition worldPosition) {
    glm::vec3 scaledPos = worldPosition.Position / VOXEL_SIZE;
    glm::ivec3 chunk = glm::ivec3(glm::floor(scaledPos / (float)CHUNK_SIZE));

    return {chunk};
}

static VoxelPosition WorldPositionToVoxel(WorldPosition worldPosition) {
    return {glm::ivec3(worldPosition.Position)};
}

static LocalVoxelPosition WorldPositionToLocalVoxel(WorldPosition worldPosition) {
    auto chunkPos = WorldPositionToChunk(worldPosition);
    auto voxelPos = WorldPositionToVoxel(worldPosition);

    glm::ivec3 localVoxelPos = voxelPos.Position - (chunkPos.Position * CHUNK_SIZE);

    localVoxelPos.x = glm::clamp(localVoxelPos.x, 0, CHUNK_SIZE - 1);
    localVoxelPos.y = glm::clamp(localVoxelPos.y, 0, CHUNK_SIZE - 1);
    localVoxelPos.z = glm::clamp(localVoxelPos.z, 0, CHUNK_SIZE - 1);

    return {localVoxelPos};
}

static WorldPosition LocalVoxelToWorldPosition(ChunkPosition chunkPos, LocalVoxelPosition localVoxelPos) {
    glm::ivec3 worldVoxelPos = chunkPos.Position * CHUNK_SIZE + localVoxelPos.Position;
    return { glm::vec3(worldVoxelPos) * VOXEL_SIZE};
}

#endif //VOXELPOSITIONING_H