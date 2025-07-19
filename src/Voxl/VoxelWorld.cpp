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

    for (int x = -HALF_WORLD_SIZE; x < HALF_WORLD_SIZE; ++x) {
        for (int z = -HALF_WORLD_SIZE; z < HALF_WORLD_SIZE; ++z) {
            auto chunk = std::make_unique<VoxelChunk>();
            chunk->GenerateTerrain(GetWorldNoise({glm::ivec3(x, 0, z)}));
            chunk->Position = glm::vec3(static_cast<float>(x) * CHUNK_SIZE * VOXEL_SIZE, 0, static_cast<float>(z) * CHUNK_SIZE * VOXEL_SIZE);
            chunk->CreateMesh();

            auto key = VoxelChunkKey(x, z);
            _chunks[key] = std::move(chunk);
        }
    }
}

void VoxelWorld::Update(float deltaTime, glm::vec3 cameraPosition, glm::vec3 cameraView) {
    RenderWorldAsync();
    RebuildChunks();

    _cameraPosition = cameraPosition;
    _cameraView = cameraView;
}

void VoxelWorld::RebuildChunks() {
    // ToDo: Will need a better implementation once we have dynamic loading
    for (auto& [key, chunk] : _chunks) {
        if (chunk->IsFlaggedForRebuild()) {
            chunk->CreateMesh();
            chunk->SetFlaggedForRebuild(false);
        }
    }
}

void VoxelWorld::RenderWorldAsync() {
    // ToDo: Will need a better implementation once we have dynamic loading
    for (auto& [key, chunk] : _chunks) {
        chunk->Update(1);

        if (chunk->ShouldRender()) {
            chunk->Render(_shader, _camera);
        }
    }
}

Neighbours VoxelWorld::CalculateChunkNeighbours(VoxelChunkKey chunkKey) {
    auto currentChunk = GetChunk(chunkKey);

    if (currentChunk == nullptr)
        return {};

    int x = static_cast<int>(currentChunk->Position.x);
    int z = static_cast<int>(currentChunk->Position.z);

    auto xNegChunk = GetChunk(VoxelChunkKey{x - 1, z});
    auto xPosChunk = GetChunk(VoxelChunkKey{x + 1, z});
    auto zNegChunk = GetChunk(VoxelChunkKey{x, z - 1});
    auto zPosChunk = GetChunk(VoxelChunkKey{x, z + 1});

    bool xNegative = true;
    bool xPositive = true;
    bool zNegative = true;
    bool zPositive = true;

    if (xNegChunk == nullptr)
        xNegative = false;
    if (xPosChunk == nullptr)
        xPositive = false;
    if (zNegChunk == nullptr)
        zNegative = false;
    if (zPosChunk == nullptr)
        zPositive = false;

    return {xNegative, xPositive, zNegative, zPositive};
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
