//
// Created by Ben Gotts on 07/07/2025.
//

#define CHUNK_SIZE (int)32
#define VOXEL_SIZE (float)0.1f

#ifndef VOXELCHUNK_H
#define VOXELCHUNK_H

#include <future>

#include "BoundingBox.h"
#include "glad/glad.h"
#include "src/Core/Camera.h"
#include "src/Core/Mesh.h"
#include "src/Core/Texture.h"
#include "src/Core/VertexBuffer.h"
#include "VoxelBlock.h"
#include "VoxelPositioning.h"
#include "VoxelTerrainGenerator.h"

struct NeighbourStates {
    bool xNeg, xPos, yNeg, yPos, zNeg, zPos;
};

struct MeshData {
    std::vector<Vertex> MeshVertices;
    std::vector<GLuint> MeshIndices;
    std::vector<Texture> MeshTextures;
};

class VoxelChunk {
public:
    VoxelChunk();
    ~VoxelChunk();

    void Setup(ChunkTerrainData terrainData);
    void Update(float deltaTime);

    void Load();
    void Unload();
    void Rebuild();

    void Render(Shader& shader, Camera& camera);
    bool ShouldRender();

    void CreateMesh();

    BoundingBox* GetBounds();

    VoxelBlock* GetVoxelBlock(WorldPosition worldPosition);
    VoxelBlock* GetVoxelBlock(LocalVoxelPosition localVoxelPosition);
    VoxelBlock* GetVoxelBlock(LocalVoxelPosition localVoxelPosition, LocalVoxelPosition& voxelBlockPosition);

    bool IsLoaded();
    bool IsSetup();

    WorldPosition GetWorldPosition();
    void SetWorldPosition(WorldPosition val);
    ChunkPosition GetChunkPosition();
private:
    void CreateVoxel(glm::vec3 position, VoxelBlockTextureData textureData, NeighbourStates neighbourStates, MeshData& outMeshData);
    void CreateMeshData(MeshData& outMeshData);

    void RecalculateBounds();
private:
    Texture ChunkSpriteSheet;

    std::vector<std::vector<std::vector<std::unique_ptr<VoxelBlock>>>> _blocks;
    std::unique_ptr<Mesh> _chunkMesh;

    WorldPosition _worldPosition = WorldPosition{0, 0, 0};
    ChunkPosition _chunkPosition = ChunkPosition{0, 0, 0};

    std::vector<Vertex> _meshVertices;
    std::vector<GLuint> _meshIndices;
    std::vector<Texture> _meshTextures;

    bool _isSetup = false;
    bool _isEmpty = false;

    bool _isLoadingChunk = false;
    bool _hasLoadedChunk = false;
    bool _hasLoadedChunkMesh = false;
    std::future<MeshData> _futureMeshData;

    BoundingBox* _boundingBox;
};

#endif //VOXELCHUNK_H
