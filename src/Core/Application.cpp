//
// Created by Ben Gotts on 03/08/2025.
//

#include "Application.h"

#include <spdlog/spdlog.h>

#include "Skybox.h"
#include "src/Voxl/VoxelWorld.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

void Application::Start() {
    _window = std::make_unique<Window>(SCREEN_WIDTH, SCREEN_HEIGHT);
    _camera = std::make_unique<Camera>(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0, 2, 0));
    _appShader = std::make_unique<Shader>("../assets/shaders/default.vert", "../assets/shaders/default.frag");

    _world = std::make_unique<VoxelWorld>(*_appShader, *_camera, 0);
    _skybox = std::make_unique<Skybox>(_camera.get());

    Physics physics = Physics(_world.get());
}

double curTime = 0.0;
double prevTime = 0.0;
double timeDiff;
unsigned int fpsCounter = 0;

static int oldState = GLFW_RELEASE;

void Application::Update() {
    curTime = glfwGetTime();
    timeDiff = curTime - prevTime;
    fpsCounter++;
    if (timeDiff >= 1.0 / 30.0) {
        std::string fps = std::to_string((1.0 / timeDiff) * fpsCounter);
        std::string ms = std::to_string((timeDiff / fpsCounter) * 1000);
        std::string newTitle = "Voxl - " + fps + "FPS / " + ms + "ms";

        glfwSetWindowTitle(_window->GetNativeWindow(), newTitle.c_str());

        prevTime = curTime;
        fpsCounter = 0;

        _camera->HandleInput(_window->GetNativeWindow());
    }

    glClearColor(0.0, 0.2, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int newState = glfwGetMouseButton(_window->GetNativeWindow(), GLFW_MOUSE_BUTTON_LEFT);
    if (newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
        RaycastHit outHit{};
        if (Physics::Raycast(_camera->Position, _camera->Direction, 100.0f, outHit)) {
            _world->PlaceVoxelBlock(outHit);
        }
    }
    oldState = newState;

    _camera->UpdateMatrix(90.0f, 0.1f, 100.0f);

    // SKYBOX
    _skybox->Render();

    // WORLD
    _appShader->Activate();
    _world->Update(curTime / 1.0f, _camera->Position, _camera->Direction);

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
