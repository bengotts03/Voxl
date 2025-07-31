//
// Created by Ben Gotts on 07/07/2025.
//


#ifndef VOXELWORLD_H
#define VOXELWORLD_H

#define CHUNK_FRAME_UPDATE_LIMIT (int)4

#include "BoundingBox.h"
#include "FastNoiseLite.h"
#include "FrustrumCuller.h"
#include "glm/glm.hpp"
#include "src/Core/Shader.h"
#include "src/Core/Camera.h"
#include "VoxelChunk.h"
#include "VoxelPositioning.h"
#include "VoxelTerrainGenerator.h"

struct RaycastHit {
    WorldPosition WorldPositionHit{};
    ChunkPosition ChunkPositionHit{};
    VoxelPosition VoxelPositionHit{};
    LocalVoxelPosition LocalVoxelPositionHit{};
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

class VoxelChunk;
class VoxelWorld {
public:
    VoxelWorld(Shader& shader, Camera& camera);
    VoxelWorld(Shader& shader, Camera& camera, int seed);
    ~VoxelWorld();

    void Init();
    void Update(float deltaTime, glm::vec3 cameraPosition, glm::vec3 cameraView);

    void RenderChunk(VoxelChunk* chunk);
    void RenderWorldAsync();
    void UpdateSetupList();
    void UpdateLoadList();
    void UpdateUnloadList();
    void UpdateVisibleChunks();
    void UpdateChunksToRender();
    void RebuildChunks();

    bool Raycast(glm::vec3 origin, glm::vec3 direction, RaycastHit& outHit, float maxDistance = 100.0f);

    VoxelChunk* GetChunk(WorldPosition worldPosition);
    VoxelChunk* GetChunk(ChunkPosition chunkPosition);
    VoxelChunk* GetChunk(VoxelChunkKey key);

    bool PlaceVoxelBlock(WorldPosition worldPosition);
    bool DestroyVoxelBlock(WorldPosition worldPosition);
private:
    std::unordered_map<VoxelChunkKey, std::unique_ptr<VoxelChunk>, VoxelChunkKeyHash> _chunks;
    std::unordered_map<VoxelChunkKey, VoxelChunk*, VoxelChunkKeyHash> _chunksToSetup;
    std::unordered_map<VoxelChunkKey, VoxelChunk*, VoxelChunkKeyHash> _chunksToLoad;
    std::unordered_map<VoxelChunkKey, VoxelChunk*, VoxelChunkKeyHash> _chunksToUnload;
    std::unordered_map<VoxelChunkKey, VoxelChunk*, VoxelChunkKeyHash> _chunksVisible;
    std::vector<VoxelChunk*> _chunksToRebuild;
    std::vector<VoxelChunk*> _chunksToRender;

    glm::vec3 _cameraPosition = glm::vec3(-1.0f, -1.0f, -1.0f);
    glm::vec3 _cameraView = glm::vec3(-1.0f, -1.0f, -1.0f);

    Shader& _shader;
    Camera& _camera;

    FrustrumCuller* _frustrumCuller = nullptr;

    int _worldSeed = 0;
    VoxelTerrainGenerator* _terrainGenerator;

    const int _chunkViewDistance = 3;
};

#endif //VOXELWORLD_H
