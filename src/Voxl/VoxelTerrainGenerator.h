//
// Created by Ben Gotts on 29/07/2025.
//

#ifndef VOXELTERRAINGENERATOR_H
#define VOXELTERRAINGENERATOR_H

#include <vector>
#include "FastNoiseLite.h"
#include "VoxelBlock.h"

struct ChunkPosition;

struct ChunkTerrainData {
    std::vector<std::vector<std::vector<std::unique_ptr<VoxelBlock>>>> Blocks;
};

class VoxelTerrainGenerator {
public:
    virtual ~VoxelTerrainGenerator() = default;

    virtual void GenerateTerrainNoise() {}
    virtual float* GetChunkNoise(ChunkPosition chunkPosition) {}

    virtual ChunkTerrainData GenerateTerrain(ChunkPosition chunkPosition) {}
protected:
    FastNoiseLite _terrainNoise;
};

#endif //VOXELTERRAINGENERATOR_H
