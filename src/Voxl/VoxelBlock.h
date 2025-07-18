//
// Created by Ben Gotts on 07/07/2025.
//

#ifndef VOXELBLOCK_H
#define VOXELBLOCK_H

enum VoxelBlockType {
    Default,
    Grass,
    Dirt,
    Stone
};

class VoxelBlock {
public:
    VoxelBlock(VoxelBlockType blockType = Default);
    ~VoxelBlock();

    bool IsActive();
    void SetActive(bool val);
private:
    bool _isActive;
    VoxelBlockType _blockType;
};

#endif //VOXELBLOCK_H
