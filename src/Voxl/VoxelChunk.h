//
// Created by Ben Gotts on 07/07/2025.
//

#define CHUNK_SIZE (int)32
#define VOXEL_SIZE (float)0.1f

#ifndef VOXELCHUNK_H
#define VOXELCHUNK_H

#include "BoundingBox.h"
#include "glad/glad.h"
#include "src/Core/Camera.h"
#include "src/Core/Mesh.h"
#include "src/Core/Texture.h"
#include "src/Core/VertexBuffer.h"
#include "VoxelBlock.h"
#include "VoxelPositioning.h"
#include "VoxelTerrainGenerator.h"

class VoxelChunk {
public:
    VoxelChunk();
    ~VoxelChunk();

    void Setup(ChunkTerrainData terrainData);
    void Update(float deltaTime);

    void Load();
    void Unload();

    void Render(Shader& shader, Camera& camera);
    bool ShouldRender();

    void CreateMesh();
    BoundingBox* GetBounds();

    VoxelBlock* GetVoxelBlock(WorldPosition worldPosition);
    VoxelBlock* GetVoxelBlock(ChunkPosition chunkPosition);
    VoxelBlock* GetVoxelBlock(LocalVoxelPosition localVoxelPosition);
    VoxelBlock* GetVoxelBlock(LocalVoxelPosition localVoxelPosition, LocalVoxelPosition& voxelBlockPosition);

    bool IsLoaded();
    bool IsSetup();

    WorldPosition GetWorldPosition();
    void SetWorldPosition(WorldPosition val);
    ChunkPosition GetChunkPosition();
private:
    void CreateVoxel(glm::vec3 position, VoxelBlockTextureData textureData, bool xNeg, bool xPos, bool yNeg, bool yPos, bool zNeg, bool zPos);
    void RecalculateBounds();
private:
    std::vector<std::vector<std::vector<std::unique_ptr<VoxelBlock>>>> _blocks;
    std::unique_ptr<Mesh> _chunkMesh;

    WorldPosition _worldPosition = WorldPosition{0, 0, 0};
    ChunkPosition _chunkPosition = ChunkPosition{0, 0, 0};

    std::vector<Vertex> _meshVertices;
    std::vector<GLuint> _meshIndices;
    std::vector<Texture> _meshTextures;

    bool _isSetup = false;
    bool _isLoaded = false;
    bool _isEmpty = false;

    BoundingBox* _boundingBox;
};

#endif //VOXELCHUNK_H
