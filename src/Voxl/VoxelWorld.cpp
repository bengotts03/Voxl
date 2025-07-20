//
// Created by Ben Gotts on 07/07/2025.
//

#include "VoxelWorld.h"
#include "VoxelChunk.h"
#include <spdlog/spdlog.h>

#include "FastNoiseLite.h"

VoxelWorld::VoxelWorld(Shader& shader, Camera& camera): _shader(shader), _camera(camera) {
    Init();
}

VoxelWorld::VoxelWorld(Shader& shader, Camera& camera, int seed) : _shader(shader), _camera(camera), _worldSeed(seed) {
    Init();
}

VoxelWorld::~VoxelWorld() {

}

void VoxelWorld::Init() {
    _chunks.clear();

    // _visibleChunksWithinViewDistance = static_cast<int>(_maxViewDistance / CHUNK_SIZE);
    // spdlog::info("View Dist: {0}", _visibleChunksWithinViewDistance);

    _camera.Position = glm::vec3(static_cast<float>(-2) * CHUNK_SIZE * VOXEL_SIZE,
                                 0,
                                 static_cast<float>(1) * CHUNK_SIZE * VOXEL_SIZE);

    // for (int x = -HALF_WORLD_SIZE; x < HALF_WORLD_SIZE; ++x) {
    //     for (int z = -HALF_WORLD_SIZE; z < HALF_WORLD_SIZE; ++z) {
    //         auto chunk = std::make_unique<VoxelChunk>();
    //         chunk->GenerateTerrain(GetWorldNoise({glm::ivec3(x, 0, z)}));
    //         chunk->SetWorldPosition(glm::vec3(static_cast<float>(x) * CHUNK_SIZE * VOXEL_SIZE, 0, static_cast<float>(z) * CHUNK_SIZE * VOXEL_SIZE));
    //         chunk->Load();
    //
    //         auto key = VoxelChunkKey(x, z);
    //         _chunks[key] = std::move(chunk);
    //     }
    // }
}

void VoxelWorld::Update(float deltaTime, glm::vec3 cameraPosition, glm::vec3 cameraView) {
    RenderWorldAsync();
    UpdateVisibleChunks();
    RebuildChunks();

    _cameraPosition = cameraPosition;
    _cameraView = cameraView;
}


void VoxelWorld::RebuildChunks() {
    // ToDo: Will need a better implementation once we have dynamic loading
    for (auto& [key, chunk] : _visibleChunks) {
        if (chunk->IsFlaggedForRebuild() && chunk->ShouldRender()) {
            chunk->CreateMesh();
            chunk->SetFlaggedForRebuild(false);
        }
    }
}

void VoxelWorld::RenderWorldAsync() {
    // ToDo: Will need a better implementation once we have dynamic loading
    for (auto& [key, chunk] : _visibleChunks) {
        chunk->Update(1);

        if (chunk->ShouldRender()) {
            chunk->Render(_shader, _camera);
        }
    }
}

float VoxelWorld::CalculateDistanceToChunkBounds(const ChunkPosition& chunkPos) const {
    // Convert chunk coordinates to world coordinates
    glm::vec3 chunkWorldPos = glm::vec3(
        static_cast<float>(chunkPos.Position.x) * CHUNK_SIZE * VOXEL_SIZE,
        0,
        static_cast<float>(chunkPos.Position.z) * CHUNK_SIZE * VOXEL_SIZE
    );

    glm::vec3 boundsMin = chunkWorldPos;
    glm::vec3 boundsMax = chunkWorldPos + glm::vec3(CHUNK_SIZE * VOXEL_SIZE, 0, CHUNK_SIZE * VOXEL_SIZE);

    float clampedX = glm::clamp(_cameraPosition.x, boundsMin.x, boundsMax.x);
    float clampedZ = glm::clamp(_cameraPosition.z, boundsMin.z, boundsMax.z);

    float dx = _cameraPosition.x - clampedX;
    float dz = _cameraPosition.z - clampedZ;

    return glm::sqrt(dx * dx + dz * dz);
}

void VoxelWorld::UpdateVisibleChunks() {
    auto currentPositionInWorld = WorldPosition{_cameraPosition};
    auto currentChunkPosition = WorldPositionToChunk(currentPositionInWorld);

    auto previouslyVisibleChunks = std::move(_visibleChunks);
    _visibleChunks.clear();

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
                        chunk->Load();

                    _visibleChunks[viewedChunkKey] = chunk.get();
                }
                else {
                    auto chunk = std::make_unique<VoxelChunk>();
                    chunk->GenerateTerrain(GetWorldNoise(viewedChunkPosition));
                    chunk->SetWorldPosition(glm::vec3(static_cast<float>(viewedChunkPosition.Position.x) * CHUNK_SIZE * VOXEL_SIZE,
                                                0,
                                                static_cast<float>(viewedChunkPosition.Position.z) * CHUNK_SIZE * VOXEL_SIZE));
                    chunk->Load();

                    // Gives a pointer of the chunk to our visibles tracker
                    _visibleChunks[viewedChunkKey] = chunk.get();
                    // Registers the chunk
                    _chunks[viewedChunkKey] = std::move(chunk);
                }
            }
        }
    }

    for (auto& [key, chunk] : previouslyVisibleChunks) {
        if (!_visibleChunks.contains(key) && chunk->IsLoaded())
            chunk->Unload();
    }
}

float* VoxelWorld::GetWorldNoise(ChunkPosition chunkPosition) {
    static float heightMap[CHUNK_SIZE * CHUNK_SIZE];

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetSeed(240503);
    noise.SetFrequency(0.01f);

    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalOctaves(4);
    noise.SetFractalLacunarity(2.2f);
    noise.SetFractalGain(0.5f);

    noise.SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
    noise.SetDomainWarpAmp(2.5f);

    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            float samplePositionX = x + (chunkPosition.Position.x * CHUNK_SIZE);
            float samplePositionZ = z + (chunkPosition.Position.z * CHUNK_SIZE);

            heightMap[x * CHUNK_SIZE + z] = noise.GetNoise(samplePositionX, samplePositionZ);
        }
    }

    return heightMap;
}

bool VoxelWorld::Raycast(glm::vec3 origin, glm::vec3 direction, RaycastHit& outHit, float maxDistance) {
    glm::vec3 normalizedDir = glm::normalize(direction);
    float stepSize = VOXEL_SIZE * 0.1f;

    for (float i = 0; i < maxDistance; i += stepSize) {
        glm::vec3 currentPos = origin + normalizedDir * i;

        // Dividing by voxel size automatically converts xyz to our voxel grid coordinates
        int x = static_cast<int>(glm::floor(currentPos.x / VOXEL_SIZE));
        int y = static_cast<int>(glm::floor(currentPos.y / VOXEL_SIZE));
        int z = static_cast<int>(glm::floor(currentPos.z / VOXEL_SIZE));

        WorldPosition worldPos = {glm::vec3(x, y, z)};
        auto chunk = GetChunk(worldPos);

        if (chunk == nullptr) {
            continue;
        }

        ChunkPosition chunkPos = WorldPositionToChunk(worldPos);
        VoxelPosition voxelPos = WorldPositionToVoxel(worldPos);
        LocalVoxelPosition localVoxelPos = WorldPositionToLocalVoxel(worldPos);

        glm::ivec3 pos = localVoxelPos.Position;

        if (pos.x >= 0 && pos.y >= 0 && pos.z >= 0 &&
            pos.x < CHUNK_SIZE && pos.y < CHUNK_SIZE && pos.z < CHUNK_SIZE) {
            auto voxel = chunk->GetVoxelBlock(localVoxelPos);

            if (voxel && voxel->IsActive()) {
                outHit = RaycastHit(worldPos, chunkPos, voxelPos, localVoxelPos);
                return true;
            }
        }
    }

    return false;
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

bool VoxelWorld::PlaceVoxelBlock(WorldPosition worldPosition) {
    auto chunk = GetChunk(worldPosition);

    if (chunk) {
        LocalVoxelPosition voxelPos{};
        auto voxel = chunk->GetVoxelBlock(WorldPositionToLocalVoxel(worldPosition), voxelPos);

        if (voxel->IsActive() == false) {

            voxel->SetActive(true);
            chunk->SetFlaggedForRebuild(true);

            return true;
        }
    }

    return false;
}

bool VoxelWorld::DestroyVoxelBlock(WorldPosition worldPosition) {
    auto chunk = GetChunk(worldPosition);

    if (chunk) {
        auto voxel = chunk->GetVoxelBlock(WorldPositionToLocalVoxel(worldPosition));
        if (voxel->IsActive() == true) {
            voxel->SetActive(false);
            chunk->SetFlaggedForRebuild(true);

            return true;
        }
    }

    return false;
}
