//
// Created by Ben Gotts on 07/07/2025.
//

#include "VoxelWorld.h"

#include <future>

#include "VoxelChunk.h"
#include <spdlog/spdlog.h>
#include "SimpleVoxelTerrainGenerator.h"
#include "BS_thread_pool.hpp"

BS::thread_pool<BS::none> VoxelWorld::ChunkPool = BS::thread_pool<BS::none>(CHUNK_FRAME_UPDATE_LIMIT);

VoxelWorld::VoxelWorld(Shader& shader, Camera& camera): _shader(shader), _camera(camera){
    Init();
}

VoxelWorld::VoxelWorld(Shader& shader, Camera& camera, int seed) : _shader(shader), _camera(camera), _worldSeed(seed) {
    Init();
}

VoxelWorld::~VoxelWorld() = default;

void VoxelWorld::Init() {
    _chunks.clear();
    _chunksToSetup.clear();
    _chunksToLoad.clear();
    _chunksToUnload.clear();
    _chunksVisible.clear();

    _terrainGenerator = new SimpleVoxelTerrainGenerator();
    _terrainGenerator->GenerateTerrainNoise();
}

void VoxelWorld::Update(float deltaTime, glm::vec3 cameraPosition, glm::vec3 cameraView) {
    RenderWorldAsync();
    UpdateSetupList();
    UpdateLoadList();
    UpdateUnloadList();
    UpdateVisibleChunks();
    RebuildChunks();

    if (cameraPosition != _cameraPosition || cameraView != _cameraView) {
        UpdateChunksToRender();
    }

    _cameraPosition = cameraPosition;
    _cameraView = cameraView;
}

void VoxelWorld::RebuildChunks() {
    int numberOfChunksToRebuild = 0;
    for (auto& chunk : _chunksToRebuild) {
        if (chunk->ShouldRender() && numberOfChunksToRebuild != CHUNK_FRAME_UPDATE_LIMIT) {
            chunk->ForceRebuild();

            numberOfChunksToRebuild++;
        }
    }

    _chunksToRebuild.clear();
}

void VoxelWorld::RenderWorldAsync() {
    for (auto& chunk : _chunksToRender) {
        chunk->Update(1);
        chunk->Render(_shader, _camera);
    }
}

void VoxelWorld::UpdateSetupList() {
    for (auto [key, chunk]: _chunksToSetup) {
        auto chunkPos = chunk->GetChunkPosition();
        chunk->Setup(_terrainGenerator->GenerateTerrain(chunkPos));

        _chunksToLoad[key] = chunk;
    }

    _chunksToSetup.clear();
}

void VoxelWorld::UpdateLoadList() {
    int numberOfChunksLoaded = 0;
    for (auto [key, chunk]: _chunksToLoad) {
        if (numberOfChunksLoaded >= CHUNK_FRAME_UPDATE_LIMIT)
            break;

        if (!chunk->IsLoaded()) {
            chunk->Load();

            numberOfChunksLoaded++;
        }
    }

    _chunksToLoad.clear();
}

void VoxelWorld::UpdateUnloadList() {
    for (auto [key, chunk]: _chunksToUnload) {
        chunk->Unload();
    }

    _chunksToUnload.clear();
}

void VoxelWorld::UpdateVisibleChunks() {
    auto currentPositionInWorld = WorldPosition{_cameraPosition};
    auto currentChunkPosition = WorldPositionToChunk(currentPositionInWorld);

    auto previouslyVisibleChunks = std::move(_chunksVisible);
    _chunksVisible.clear();

    for (int z = -_chunkViewDistance; z < _chunkViewDistance; ++z) {
        for (int x = -_chunkViewDistance; x < _chunkViewDistance; ++x) {
            auto viewedChunkPosition = ChunkPosition(currentChunkPosition.Position.x + x, 0, currentChunkPosition.Position.z + z);
            VoxelChunkKey viewedChunkKey{viewedChunkPosition.Position.x, viewedChunkPosition.Position.z};

            float chunkDistance = std::sqrt(static_cast<float>(x * x + z * z));
            bool isVisible = chunkDistance <= static_cast<float>(_chunkViewDistance);

            if (isVisible) {
                if (_chunks.contains(viewedChunkKey)) {
                    auto& chunk = _chunks[viewedChunkKey];

                    if (!chunk->IsLoaded())
                        _chunksToLoad[viewedChunkKey] = chunk.get();

                    _chunksVisible[viewedChunkKey] = chunk.get();
                }
                else {
                    auto chunk = std::make_unique<VoxelChunk>();
                    chunk->SetWorldPosition(glm::vec3(static_cast<float>(viewedChunkPosition.Position.x) * CHUNK_SIZE * VOXEL_SIZE,
                              0,
                              static_cast<float>(viewedChunkPosition.Position.z) * CHUNK_SIZE * VOXEL_SIZE));
                    _chunksToSetup[viewedChunkKey] = chunk.get();

                    // Gives a pointer of the chunk to our visibles tracker
                    _chunksVisible[viewedChunkKey] = chunk.get();

                    // Registers the chunk
                    _chunks[viewedChunkKey] = std::move(chunk);
                }
            }
        }
    }

    for (auto& [key, chunk] : previouslyVisibleChunks)
        if (!_chunksVisible.contains(key) && chunk->IsLoaded())
            _chunksToUnload[key] = chunk;
}

void VoxelWorld::UpdateChunksToRender() {
    _chunksToRender.clear();

    for (auto [key, chunk] : _chunksVisible) {
        if (chunk->IsLoaded() && chunk->IsSetup()) {
            if (chunk->ShouldRender()) {
                _chunksToRender.push_back(chunk);
            }
        }
    }
}

VoxelChunk* VoxelWorld::GetChunk(WorldPosition worldPosition) {
    auto chunkPos = WorldPositionToChunk(worldPosition);

    const int chunkXPos = chunkPos.Position.x;
    const int chunkZPos = chunkPos.Position.z;

    auto key = VoxelChunkKey(chunkXPos, chunkZPos);
    auto iterator = _chunks.find(key);

    if (iterator != _chunks.end()) {
        return iterator->second.get();
    }

    return nullptr;
}

VoxelChunk* VoxelWorld::GetChunk(ChunkPosition chunkPosition) {
    auto key = VoxelChunkKey(chunkPosition.Position.x, chunkPosition.Position.z);
    auto iterator = _chunks.find(key);

    if (iterator != _chunks.end()) {
        return iterator->second.get();
    }

    spdlog::warn("Chunk not found at: {0},{1}", chunkPosition.Position.x, chunkPosition.Position.z);
    return nullptr;
}

VoxelChunk* VoxelWorld::GetChunk(VoxelChunkKey key) {
    auto iterator = _chunks.find(key);

    if (iterator != _chunks.end()) {
        return iterator->second.get();
    }

    return nullptr;
}

bool VoxelWorld::PlaceVoxelBlock(RaycastHit hit) {
    auto chunk = GetChunk(hit.WorldPositionHit);
    if (!chunk)
        return false;

    LocalVoxelPosition voxelPos{};
    auto voxel = chunk->GetVoxelBlock(WorldPositionToLocalVoxel(hit.WorldPositionHit), voxelPos);

    if (voxel->IsActive() == false)
        return false;


    glm::vec3 normal = hit.HitNormal;
    auto voxelPlacementPos = glm::vec3(voxelPos.Position) + normal;

    if (!chunk->GetVoxelBlock(WorldPositionToLocalVoxel(voxelPlacementPos)))
        return false;

    auto newVoxel = chunk->GetVoxelBlock(LocalVoxelPosition(voxelPlacementPos));

    if (newVoxel == nullptr || newVoxel->IsActive() == true)
        return false;

    newVoxel->SetActive(true);
    _chunksToRebuild.push_back(chunk);

    return true;
}

bool VoxelWorld::DestroyVoxelBlock(WorldPosition worldPosition) {
    auto chunk = GetChunk(worldPosition);
    if (!chunk)
        return false;

    auto voxel = chunk->GetVoxelBlock(worldPosition);
    if (voxel->IsActive() == false)
        return false;

    voxel->SetActive(false);
    _chunksToRebuild.push_back(chunk);

    return true;
}
