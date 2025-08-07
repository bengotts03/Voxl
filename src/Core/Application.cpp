//
// Created by Ben Gotts on 03/08/2025.
//

#include "Application.h"

#include <spdlog/spdlog.h>

#include "Input.h"
#include "Performance.h"
#include "../Rendering/Skybox.h"
#include "src/Debug/Gizmos.h"
#include "src/Voxl/VoxelWorld.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

void Application::Start() {
    _window = std::make_unique<Window>(SCREEN_WIDTH, SCREEN_HEIGHT);
    _camera = std::make_unique<Camera>(_window.get(), SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0, 2, 0));
    _appShader = std::make_unique<Shader>("../assets/shaders/default.vert", "../assets/shaders/default.frag");

    _world = std::make_unique<VoxelWorld>(*_appShader, *_camera, 0);
    _skybox = std::make_unique<Skybox>(_camera.get());

    Input(_window.get());
    Physics physics(_world.get());

    Time();
    Performance();
    Gizmos(_camera.get());
}

static int oldState = GLFW_RELEASE;

void Application::Update() {
    Time::Update();
    Performance::Update();
    Input::UpdateStates();

    glClearColor(0.0, 0.2, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DisplayPerformanceStats();

    _camera->HandleInput();

    RaycastHit outHit;
    if (Input::GetMouseButtonDown(MouseButton::RIGHT)) {
        if (Physics::Raycast(_camera->Position, _camera->Direction, 100.0f, outHit)) {
            _world->PlaceVoxelBlock(outHit);
        }
    }
    if (Input::GetMouseButtonDown(MouseButton::LEFT)) {
        if (Physics::Raycast(_camera->Position, _camera->Direction, 100.0f, outHit)) {
            _world->DestroyVoxelBlock(outHit.WorldPositionHit);
        }
    }

    _camera->UpdateMatrix(90.0f, 0.1f, 100.0f);

    Gizmos::StartRender();
        Gizmos::DrawLine(glm::vec3{-5, 0, 0}, glm::vec3{5, 0, 0});
    Gizmos::EndRender();

    // SKYBOX
    _skybox->Render();

    // WORLD
    _appShader->Activate();
    _world->Update(Time::NormalTime / 1.0, _camera->Position, _camera->Direction);

    glfwSwapBuffers(_window->GetNativeWindow());
    glfwPollEvents();
}

void Application::Shutdown() {
    _appShader->Delete();
}

Window& Application::GetWindow() {
    return *_window;
}

Camera& Application::GetCamera() {
    return *_camera;
}

void Application::DisplayPerformanceStats() {
    std::string fps = std::to_string(Performance::AverageFPS);
    std::string ms = std::to_string(Performance::MS);

    std::string newTitle = "Voxl - " + fps + "FPS / " + ms + "ms";
    glfwSetWindowTitle(_window->GetNativeWindow(), newTitle.c_str());
}
