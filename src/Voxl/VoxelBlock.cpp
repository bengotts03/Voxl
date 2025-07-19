//
// Created by Ben Gotts on 07/07/2025.
//

#include "VoxelBlock.h"

VoxelBlock::VoxelBlock(VoxelBlockType blockType) : _blockType(blockType){
}

VoxelBlock::~VoxelBlock() = default;

bool VoxelBlock::IsActive() {
    return _isActive;
}

void VoxelBlock::SetActive(bool val) {
    if (_isActive != val)
        _isActive = val;
}

bool VoxelBlock::GetBlockType() {
    return _blockType;
}

void VoxelBlock::SetBlockType(VoxelBlockType val) {
    if (_blockType != val)
        _blockType = val;
}

glm::vec3 VoxelBlock::GetBlockColour() {
    switch (_blockType) {
        case BLOCK_TYPE_DEFAULT:
            return {1.0f, 1.0f, 1.0f};
            break;
        case BLOCK_TYPE_GRASS:
            return {0.61f, 0.8f, 0.48f};
            break;
        case BLOCK_TYPE_DIRT:
            return {0.59f, 0.36f, 0.15f};
            break;
        case BLOCK_TYPE_STONE:
            return {0.56f, 0.56f, 0.56f};
            break;
    }
    return {0, 1, 1};
}
