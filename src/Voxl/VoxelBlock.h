//
// Created by Ben Gotts on 07/07/2025.
//

#ifndef VOXELBLOCK_H
#define VOXELBLOCK_H

#include "glm/glm.hpp"

struct VoxelBlockTextureData {
    glm::vec2 SideCoords;
    glm::vec2 TopCoords;
    glm::vec2 BottomCoords;

    VoxelBlockTextureData(glm::vec2 coords) : SideCoords(coords), TopCoords({-1, -1}), BottomCoords({-1, -1}) {}
    VoxelBlockTextureData(glm::vec2 sideCoords, glm::vec2 topCoords) : SideCoords(sideCoords), TopCoords(topCoords) {}
    VoxelBlockTextureData(glm::vec2 sideCoords, glm::vec2 topCoords, glm::vec2 bottomCoords) : SideCoords(sideCoords), TopCoords(topCoords), BottomCoords(bottomCoords) {}
};

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

    VoxelBlockTextureData GetBlockTextureCoords();
private:
    bool _isActive;
    VoxelBlockType _blockType;
};

#endif //VOXELBLOCK_H
