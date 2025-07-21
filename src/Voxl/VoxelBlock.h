//
// Created by Ben Gotts on 07/07/2025.
//

#ifndef VOXELBLOCK_H
#define VOXELBLOCK_H

#include "glm/glm.hpp"

enum VoxelBlockType {
    BLOCK_TYPE_DEFAULT,
    BLOCK_TYPE_GRASS,
    BLOCK_TYPE_DIRT,
    BLOCK_TYPE_STONE
};

class VoxelBlock {
public:
    VoxelBlock(VoxelBlockType blockType = BLOCK_TYPE_GRASS);
    ~VoxelBlock();

    bool IsActive();
    void SetActive(bool val);

    VoxelBlockType GetBlockType();
    void SetBlockType(VoxelBlockType val);

    glm::vec3 GetBlockColour();
private:
    bool _isActive;
    VoxelBlockType _blockType;
};

#endif //VOXELBLOCK_H
