//
// Created by Ben Gotts on 29/07/2025.
//

#ifndef SIMPLEVOXELTERRAINGENERATOR_H
#define SIMPLEVOXELTERRAINGENERATOR_H

#include "../VoxelPositioning.h"
#include "VoxelTerrainGenerator.h"

class SimpleVoxelTerrainGenerator : public VoxelTerrainGenerator {
public:
    virtual void GenerateTerrainNoise() override;
    virtual float* GetChunkNoise(ChunkPosition chunkPosition) override;

    virtual ChunkTerrainData GenerateTerrain(ChunkPosition chunkPosition) override;
};

#endif //SIMPLEVOXELTERRAINGENERATOR_H
