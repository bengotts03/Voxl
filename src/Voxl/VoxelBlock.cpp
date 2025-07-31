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

VoxelBlockType VoxelBlock::GetBlockType() {
    return _blockType;
}

void VoxelBlock::SetBlockType(VoxelBlockType val) {
    if (_blockType != val)
        _blockType = val;
}

VoxelBlockTextureData VoxelBlock::GetBlockTextureCoords() {
    switch (_blockType) {
        case BLOCK_TYPE_DEFAULT:
            break;
        case BLOCK_TYPE_GRASS:
            return VoxelBlockTextureData({7, 5}, {6, 8});
        case BLOCK_TYPE_DIRT:
            return VoxelBlockTextureData({7, 4});
        case BLOCK_TYPE_STONE:
            return VoxelBlockTextureData({3, 5});
    }
}
