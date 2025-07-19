//
// Created by Ben Gotts on 07/07/2025.
//

#include "VoxelChunk.h"

#include <glm/vec3.hpp>
#include <spdlog/spdlog.h>
#include "src/Core/Mesh.h"
#include "random"

VoxelChunk::VoxelChunk() {
    _blocks.resize(CHUNK_SIZE);

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        _blocks[x].resize(CHUNK_SIZE);

        for (int y = 0; y < CHUNK_SIZE; ++y) {
            _blocks[x][y].resize(CHUNK_SIZE);

            for (int z = 0; z < CHUNK_SIZE; ++z) {
                _blocks[x][y][z] = std::make_unique<VoxelBlock>(BLOCK_TYPE_STONE);
            }
        }
    }
}

VoxelChunk::~VoxelChunk() {
}

void VoxelChunk::Setup() {

}

void VoxelChunk::Update(float deltaTime) {
    if (!IsLoaded())
        return;

    _chunkMesh->Position = Position;

    if (_chunkMesh->GetNumVerts() == 0 && _chunkMesh->GetNumIndices() == 0)
        _isEmpty = true;
    else
        _isEmpty = false;
}

void VoxelChunk::Render(Shader& shader, Camera& camera) {
    if (!IsLoaded())
        return;

    shader.SetFloat("scale", VOXEL_SIZE);
    _chunkMesh->Draw(shader, camera);
}

bool VoxelChunk::ShouldRender() {
    return !_isEmpty;
}

// Todo: Might be good to cache the terrain data for loading and then have a separate method for loading which the VoxelWorld class
void VoxelChunk::GenerateTerrain(float heightMap[CHUNK_SIZE * CHUNK_SIZE]) {
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            float heightMapNoise = heightMap[x * CHUNK_SIZE + z];
            float normalizedHeight = (heightMapNoise + 1.0f) * 0.5f;
            int height = int(normalizedHeight * CHUNK_SIZE);

            for (int y = 0; y < height; y++) {
                auto currentBlock = _blocks[x][y][z].get();
                currentBlock->SetActive(true);

                if (y < height && y > height / 2)
                    currentBlock->SetBlockType(BLOCK_TYPE_DIRT);
                if (y == height - 1)
                    currentBlock->SetBlockType(BLOCK_TYPE_GRASS);
            }
        }
    }
}

void VoxelChunk::CreateVoxel(glm::vec3 position, glm::vec3 colour, bool xNeg, bool xPos, bool yNeg, bool yPos, bool zNeg, bool zPos) {
    Vertex vertices[24];

    GLuint vertexOffset = static_cast<GLuint>(_meshVertices.size());
    std::vector<GLuint> offsetIndices;

    // Front face (zNeg)
    GLuint frontIndices[] = { 0, 3, 2, 2, 1, 0 };
    // Back face (zPos)
    GLuint backIndices[] = { 4, 5, 6, 6, 7, 4 };
    // Left face (xNeg)
    GLuint leftIndices[] = { 11, 8, 9, 9, 10, 11 };
    // Right face (xPos)
    GLuint rightIndices[] = { 12, 13, 14, 14, 15, 12 };
    // Bottom face (yNeg)
    GLuint bottomIndices[] = { 16, 17, 18, 18, 19, 16 };
    // Top face (yPos)
    GLuint topIndices[] = { 20, 21, 22, 22, 23, 20 };

    // Front face (zNeg)
    if (zNeg == true) {
        glm::vec3 normal = glm::vec3(0.0f, 0.0f, -1.0f);

        vertices[0] = Vertex({{-0.5f, -0.5f, -0.5f}, colour, {0.0f, 0.0f}, normal});
        vertices[1] = Vertex({{ 0.5f, -0.5f, -0.5f}, colour, {1.0f, 0.0f}, normal});
        vertices[2] = Vertex({{ 0.5f,  0.5f, -0.5f}, colour, {1.0f, 1.0f}, normal});
        vertices[3] = Vertex({{-0.5f,  0.5f, -0.5f}, colour, {0.0f, 1.0f}, normal});

        for (auto idx : frontIndices)
            offsetIndices.push_back(idx + vertexOffset);
    }

    // Back face (zPos)
    if (zPos == true) {
        glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);

        vertices[4] = Vertex({{-0.5f, -0.5f,  0.5f}, colour, {0.0f, 0.0f}, normal});
        vertices[5] = Vertex({{ 0.5f, -0.5f,  0.5f}, colour, {1.0f, 0.0f}, normal});
        vertices[6] = Vertex({{ 0.5f,  0.5f,  0.5f}, colour, {1.0f, 1.0f}, normal});
        vertices[7] = Vertex({{-0.5f,  0.5f,  0.5f}, colour, {0.0f, 1.0f}, normal});

        for (auto idx : backIndices)
            offsetIndices.push_back(idx + vertexOffset);
    }

    // Left face (xNeg)
    if (xNeg == true) {
        glm::vec3 normal = glm::vec3(-1.0f, 0.0f, 0.0f);

        vertices[8]  = Vertex({{-0.5f,  0.5f, -0.5f}, colour, {0.0f, 0.0f}, normal});
        vertices[9]  = Vertex({{-0.5f, -0.5f, -0.5f}, colour, {1.0f, 0.0f}, normal});
        vertices[10] = Vertex({{-0.5f, -0.5f,  0.5f}, colour, {1.0f, 1.0f}, normal});
        vertices[11] = Vertex({{-0.5f,  0.5f,  0.5f}, colour, {0.0f, 1.0f}, normal});

        for (auto idx : leftIndices)
            offsetIndices.push_back(idx + vertexOffset);
    }

    // Right face (xPos)
    if (xPos == true) {
        glm::vec3 normal = glm::vec3(1.0f, 0.0f, 0.0f);

        vertices[12] = Vertex({{ 0.5f, -0.5f, -0.5f}, colour, {0.0f, 0.0f}, normal});
        vertices[13] = Vertex({{ 0.5f,  0.5f, -0.5f}, colour, {1.0f, 0.0f}, normal});
        vertices[14] = Vertex({{ 0.5f,  0.5f,  0.5f}, colour, {1.0f, 1.0f}, normal});
        vertices[15] = Vertex({{ 0.5f, -0.5f,  0.5f}, colour, {0.0f, 1.0f}, normal});

        for (auto idx : rightIndices)
            offsetIndices.push_back(idx + vertexOffset);
    }

    // Bottom face (yNeg)
    if (yNeg == true) {
        glm::vec3 normal = glm::vec3(0.0f, -1.0f, 0.0f);

        vertices[16] = Vertex({{-0.5f, -0.5f, -0.5f}, colour, {0.0f, 0.0f}, normal});
        vertices[17] = Vertex({{ 0.5f, -0.5f, -0.5f}, colour, {1.0f, 0.0f}, normal});
        vertices[18] = Vertex({{ 0.5f, -0.5f,  0.5f}, colour, {1.0f, 1.0f}, normal});
        vertices[19] = Vertex({{-0.5f, -0.5f,  0.5f}, colour, {0.0f, 1.0f}, normal});

        for (auto idx : bottomIndices)
            offsetIndices.push_back(idx + vertexOffset);
    }

    // Top face (yPos)
    if (yPos == true) {
        glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

        vertices[20] = Vertex({{ 0.5f,  0.5f, -0.5f}, colour, {0.0f, 0.0f}, normal});
        vertices[21] = Vertex({{-0.5f,  0.5f, -0.5f}, colour, {1.0f, 0.0f}, normal});
        vertices[22] = Vertex({{-0.5f,  0.5f,  0.5f}, colour, {1.0f, 1.0f}, normal});
        vertices[23] = Vertex({{ 0.5f,  0.5f,  0.5f}, colour, {0.0f, 1.0f}, normal});

        for (auto idx : topIndices)
            offsetIndices.push_back(idx + vertexOffset);
    }

    for (auto& v : vertices)
        v.position += position;

    _meshVertices.insert(_meshVertices.end(), std::begin(vertices), std::end(vertices));
    _meshIndices.insert(_meshIndices.end(), offsetIndices.begin(), offsetIndices.end());
}

void VoxelChunk::CreateMesh() {
    _meshVertices.clear();
    _meshIndices.clear();
    _meshTextures.clear();

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                if (_blocks[x][y][z]->IsActive() == false)
                    continue;

                bool xNegative = true;
                bool xPositive = true;
                bool yNegative = true;
                bool yPositive = true;
                bool zNegative = true;
                bool zPositive = true;

                if (x > 0)
                    xNegative = !_blocks[x - 1][y][z]->IsActive();
                if (x < CHUNK_SIZE - 1)
                    xPositive = !_blocks[x + 1][y][z]->IsActive();

                if (y > 0)
                    yNegative = !_blocks[x][y - 1][z]->IsActive();
                if (y < CHUNK_SIZE - 1)
                    yPositive = !_blocks[x][y + 1][z]->IsActive();

                if (z > 0)
                    zNegative = !_blocks[x][y][z - 1]->IsActive();
                if (z < CHUNK_SIZE - 1)
                    zPositive = !_blocks[x][y][z + 1]->IsActive();

                CreateVoxel(glm::vec3(x,y,z), _blocks[x][y][z]->GetBlockColour(), xNegative, xPositive, yNegative,
                    yPositive, zNegative, zPositive);
            }
        }
    }

    _meshTextures.emplace_back("../assets/textures/WhitePixel.png", "diffuse", 0);
    _chunkMesh = new Mesh(_meshVertices, _meshIndices, _meshTextures);
}

VoxelBlock* VoxelChunk::GetVoxelBlock(WorldPosition worldPosition) {
    ChunkPosition chunkPosition = WorldPositionToChunk(worldPosition);
    return GetVoxelBlock(chunkPosition);
}
VoxelBlock* VoxelChunk::GetVoxelBlock(ChunkPosition chunkPosition) {
    glm::ivec3 pos = chunkPosition.Position;

    if (pos.x >= 0 && pos.y >= 0 && pos.z >= 0 &&
        pos.x < CHUNK_SIZE && pos.y < CHUNK_SIZE && pos.z < CHUNK_SIZE)
        return _blocks[pos.x][pos.y][pos.z].get();

    return nullptr;
}
VoxelBlock* VoxelChunk::GetVoxelBlock(LocalVoxelPosition localVoxelPosition) {
    glm::ivec3 pos = localVoxelPosition.Position;

    if (pos.x >= 0 && pos.y >= 0 && pos.z >= 0 &&
        pos.x < CHUNK_SIZE && pos.y < CHUNK_SIZE && pos.z < CHUNK_SIZE)
        return _blocks[pos.x][pos.y][pos.z].get();

    return nullptr;
}
VoxelBlock* VoxelChunk::GetVoxelBlock(LocalVoxelPosition localVoxelPosition, LocalVoxelPosition& voxelBlockPosition) {
    glm::ivec3 pos = localVoxelPosition.Position;

    if (pos.x >= 0 && pos.y >= 0 && pos.z >= 0 &&
        pos.x < CHUNK_SIZE && pos.y < CHUNK_SIZE && pos.z < CHUNK_SIZE) {
        voxelBlockPosition = {glm::ivec3(pos.x, pos.y, pos.z)};
        return _blocks[pos.x][pos.y][pos.z].get();
    }

    return nullptr;
}

bool VoxelChunk::IsFlaggedForRebuild() {
    return _isFlaggedForRebuild;
}

void VoxelChunk::SetFlaggedForRebuild(bool val) {
    _isFlaggedForRebuild = val;
}

bool VoxelChunk::IsLoaded() {
    return _isLoaded && _chunkMesh != nullptr;
}

void VoxelChunk::SetLoaded(bool val) {
    _isLoaded = val;
}

