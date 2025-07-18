//
// Created by Ben Gotts on 07/07/2025.
//

#define CHUNK_SIZE (int)32
#define VOXEL_SIZE (float)0.1f

#ifndef VOXELCHUNK_H
#define VOXELCHUNK_H

#include "glad/glad.h"
#include "src/Core/Camera.h"
#include "src/Core/Mesh.h"
#include "src/Core/Texture.h"
#include "src/Core/VertexBuffer.h"
#include "VoxelBlock.h"
#include "VoxelPositioning.h"

class VoxelChunk {
public:
    VoxelChunk();
    ~VoxelChunk();

    void Setup();
    void Update(float deltaTime);
    void Render(Shader& shader, Camera& camera);

    void GenerateTerrain(float heightMap[CHUNK_SIZE * CHUNK_SIZE]);

    void CreateVoxel(glm::vec3 position, glm::vec3 colour, bool xNeg, bool xPos, bool yNeg, bool yPos, bool zNeg, bool zPos);
    void CreateMesh(bool xNeg, bool xPos, bool zNeg, bool zPos);

    VoxelBlock* GetVoxelBlock(WorldPosition worldPosition);
    VoxelBlock* GetVoxelBlock(ChunkPosition chunkPosition);
    VoxelBlock* GetVoxelBlock(LocalVoxelPosition localVoxelPosition);
    VoxelBlock* GetVoxelBlock(LocalVoxelPosition localVoxelPosition, LocalVoxelPosition& voxelBlockPosition);
    // bool DestroyVoxelBlock(unsigned int x, unsigned int y, unsigned int z);

    bool IsFlaggedForRebuild();
    void SetFlaggedForRebuild(bool val);

    bool IsLoaded();
    void SetLoaded(bool val);

    glm::vec3 Position = glm::vec3(0, 0, 0);
private:
    std::vector<std::vector<std::vector<std::unique_ptr<VoxelBlock>>>> _blocks;
    Mesh* _chunkMesh;

    std::vector<Vertex> _meshVertices;
    std::vector<GLuint> _meshIndices;
    std::vector<Texture> _meshTextures;

    bool _isFlaggedForRebuild = false;
    bool _isLoaded = false;
};

#endif //VOXELCHUNK_H
