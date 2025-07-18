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
