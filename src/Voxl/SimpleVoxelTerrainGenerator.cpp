//
// Created by Ben Gotts on 29/07/2025.
//

#include "SimpleVoxelTerrainGenerator.h"

#include "VoxelChunk.h"

void SimpleVoxelTerrainGenerator::GenerateTerrainNoise() {
    _terrainNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    _terrainNoise.SetSeed(240503);
    _terrainNoise.SetFrequency(0.01f);

    _terrainNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    _terrainNoise.SetFractalOctaves(4);\
    _terrainNoise.SetFractalLacunarity(2.2f);
    _terrainNoise.SetFractalGain(0.5f);

    _terrainNoise.SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
    _terrainNoise.SetDomainWarpAmp(2.5f);
}

float* SimpleVoxelTerrainGenerator::GetChunkNoise(ChunkPosition chunkPosition) {
    static float heightMap[CHUNK_SIZE * CHUNK_SIZE];

    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            float samplePositionX = static_cast<float>(x) + static_cast<float>(chunkPosition.Position.x) * CHUNK_SIZE;
            float samplePositionZ = static_cast<float>(z) + static_cast<float>(chunkPosition.Position.z) * CHUNK_SIZE;

            heightMap[x * CHUNK_SIZE + z] = _terrainNoise.GetNoise(samplePositionX, samplePositionZ);
        }
    }

    return heightMap;
}

ChunkTerrainData SimpleVoxelTerrainGenerator::GenerateTerrain(ChunkPosition chunkPosition) {
    float *heightMap = GetChunkNoise(chunkPosition);

    auto chunkData = ChunkTerrainData();
    chunkData.Blocks.resize(CHUNK_SIZE);

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        chunkData.Blocks[x].resize(CHUNK_SIZE);

        for (int y = 0; y < CHUNK_SIZE; ++y) {
            chunkData.Blocks[x][y].resize(CHUNK_SIZE);

            for (int z = 0; z < CHUNK_SIZE; ++z) {
                chunkData.Blocks[x][y][z] = std::make_unique<VoxelBlock>(BLOCK_TYPE_STONE);
            }
        }
    }

    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            float heightMapNoise = heightMap[x * CHUNK_SIZE + z];
            float normalizedHeight = (heightMapNoise + 1.0f) * 0.5f;
            int height = int(normalizedHeight * CHUNK_SIZE);

            for (int y = 0; y < height; y++) {
                auto currentBlock = chunkData.Blocks[x][y][z].get();
                currentBlock->SetActive(true);

                if (y < height && y > height / 2)
                    currentBlock->SetBlockType(BLOCK_TYPE_DIRT);
                if (y == height - 1)
                    currentBlock->SetBlockType(BLOCK_TYPE_GRASS);
            }
        }
    }

    return chunkData;
}
