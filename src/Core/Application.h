//
// Created by Ben Gotts on 03/08/2025.
//

#ifndef APPLICATION_H
#define APPLICATION_H
#include "Window.h"
#include "Camera.h"
#include "src/Rendering/Skybox.h"
#include "src/Voxl/VoxelWorld.h"
#include "Time.h"

class Application {
public:
    void Start();
    void Update();
    void Shutdown();

    Window& GetWindow();
    Camera& GetCamera();
private:
    void DisplayPerformanceStats();
private:
    std::unique_ptr<Window> _window;
    std::unique_ptr<Camera> _camera;
    std::unique_ptr<Shader> _appShader;

    std::unique_ptr<VoxelWorld> _world;
    std::unique_ptr<Skybox> _skybox;
};

#endif //APPLICATION_H
