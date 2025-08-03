#include <iostream>
#include <spdlog/spdlog.h>

#include "Core/Application.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Core/Shader.h"
#include "Core/Camera.h"
#include "Core/Mesh.h"
#include "Core/Skybox.h"
#include "Core/Texture.h"
#include "Core/VertexBuffer.h"
#include "Physics/Physics.h"
#include "Voxl/VoxelWorld.h"

Camera* camera;

int main() {
    Application app;

    app.Start();

    while (!glfwWindowShouldClose(app.GetWindow().GetNativeWindow()))
    {
        app.Update();
    }

    app.Shutdown();

    return 0;
}
