//
// Created by Ben Gotts on 07/07/2025.
//

#define WORLD_SIZE (int)6
#define HALF_WORLD_SIZE (WORLD_SIZE / 2)
#include <tuple>
#include <tuple>

#ifndef VOXELWORLD_H
#define VOXELWORLD_H

#include "glm/glm.hpp"
#include "src/Core/Shader.h"
#include "src/Core/Camera.h"
#include "VoxelChunk.h"
#include "VoxelPositioning.h"

struct RaycastHit {
    WorldPosition WorldPositionHit;
    ChunkPosition ChunkPositionHit;
    VoxelPosition VoxelPositionHit;
    LocalVoxelPosition LocalVoxelPositionHit;
};

struct VoxelChunkKey {
    int x;
    int z;

    bool operator==(const VoxelChunkKey& other) const {
        return x == other.x && z == other.z;
    }
};

struct VoxelChunkKeyHash {
    std::size_t operator()(const VoxelChunkKey& key) const {
        return std::hash<int>()(key.x) ^ (std::hash<int>()(key.z) << 1);
    }
};

struct Neighbours {
    bool xNeg;
    bool xPos;
    bool zNeg;
    bool zPos;
};

class VoxelChunk;
class VoxelWorld {
public:
    VoxelWorld(Shader& shader, Camera& camera);
    VoxelWorld(Shader& shader, Camera& camera, int seed);
    ~VoxelWorld();

    void Init();
    void Update(float deltaTime, glm::vec3 cameraPosition, glm::vec3 cameraView);

    void RebuildChunks();
    void RenderWorldAsync();
    Neighbours CalculateChunkNeighbours(VoxelChunkKey chunkKey);

    float* GetWorldNoise(ChunkPosition chunkPosition);

    bool Raycast(glm::vec3 origin, glm::vec3 direction, RaycastHit& outHit, float maxDistance = 100.0f);

    VoxelChunk* GetChunk(WorldPosition worldPosition);
    VoxelChunk* GetChunk(ChunkPosition chunkPosition);
    VoxelChunk* GetChunk(VoxelChunkKey key);

    bool PlaceVoxelBlock(WorldPosition worldPosition);
    bool DestroyVoxelBlock(WorldPosition worldPosition);
private:
    // std::vector<std::unique_ptr<VoxelChunk>> _chunks;
    std::unordered_map<VoxelChunkKey, std::unique_ptr<VoxelChunk>, VoxelChunkKeyHash> _chunks;

    glm::vec3 _cameraPosition = glm::vec3(-1.0f, -1.0f, -1.0f);
    glm::vec3 _cameraView = glm::vec3(-1.0f, -1.0f, -1.0f);

    Shader& _shader;
    Camera& _camera;

    int _worldSeed = 0;
};

#endif //VOXELWORLD_H
