#include <iostream>
#include <spdlog/spdlog.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Core/Shader.h"
#include "Core/Camera.h"
#include "Core/Mesh.h"
#include "Core/Texture.h"
#include "Core/VertexBuffer.h"
#include "Voxl/VoxelWorld.h"

GLFWwindow* window;
const unsigned int width = 800;
const unsigned int height = 800;
Camera* camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    camera->Refresh(width, height);
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, "Voxl", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    auto shader = Shader("../assets/shaders/default.vert", "../assets/shaders/default.frag");
    camera = new Camera(width, height, glm::vec3(0, 0, -2));

    auto skyboxShader = Shader("../assets/shaders/skybox.vert", "../assets/shaders/skybox.frag");
    Vertex skyboxVertices[] = {
        // Front face
        {{-1.0f, -1.0f,  1.0f}, {1,1,1}, {0,0}, {0,0,-1}},
        {{ 1.0f, -1.0f,  1.0f}, {1,1,1}, {1,0}, {0,0,-1}},
        {{ 1.0f,  1.0f,  1.0f}, {1,1,1}, {1,1}, {0,0,-1}},
        {{-1.0f,  1.0f,  1.0f}, {1,1,1}, {0,1}, {0,0,-1}},

        // Back face
        {{-1.0f, -1.0f, -1.0f}, {1,1,1}, {1,0}, {0,0,1}},
        {{-1.0f,  1.0f, -1.0f}, {1,1,1}, {1,1}, {0,0,1}},
        {{ 1.0f,  1.0f, -1.0f}, {1,1,1}, {0,1}, {0,0,1}},
        {{ 1.0f, -1.0f, -1.0f}, {1,1,1}, {0,0}, {0,0,1}},

        // Left face
        {{-1.0f, -1.0f, -1.0f}, {1,1,1}, {0,0}, {-1,0,0}},
        {{-1.0f, -1.0f,  1.0f}, {1,1,1}, {1,0}, {-1,0,0}},
        {{-1.0f,  1.0f,  1.0f}, {1,1,1}, {1,1}, {-1,0,0}},
        {{-1.0f,  1.0f, -1.0f}, {1,1,1}, {0,1}, {-1,0,0}},

        // Right face
        {{ 1.0f, -1.0f, -1.0f}, {1,1,1}, {1,0}, {1,0,0}},
        {{ 1.0f,  1.0f, -1.0f}, {1,1,1}, {1,1}, {1,0,0}},
        {{ 1.0f,  1.0f,  1.0f}, {1,1,1}, {0,1}, {1,0,0}},
        {{ 1.0f, -1.0f,  1.0f}, {1,1,1}, {0,0}, {1,0,0}},

        // Top face
        {{-1.0f,  1.0f, -1.0f}, {1,1,1}, {0,1}, {0,1,0}},
        {{-1.0f,  1.0f,  1.0f}, {1,1,1}, {0,0}, {0,1,0}},
        {{ 1.0f,  1.0f,  1.0f}, {1,1,1}, {1,0}, {0,1,0}},
        {{ 1.0f,  1.0f, -1.0f}, {1,1,1}, {1,1}, {0,1,0}},

        // Bottom face
        {{-1.0f, -1.0f, -1.0f}, {1,1,1}, {1,1}, {0,-1,0}},
        {{ 1.0f, -1.0f, -1.0f}, {1,1,1}, {0,1}, {0,-1,0}},
        {{ 1.0f, -1.0f,  1.0f}, {1,1,1}, {0,0}, {0,-1,0}},
        {{-1.0f, -1.0f,  1.0f}, {1,1,1}, {1,0}, {0,-1,0}}
    };
    unsigned int skyboxIndices[] = {
        // Front face
        0, 1, 2,   2, 3, 0,

        // Back face
        4, 5, 6,   6, 7, 4,

        // Left face
        8, 9, 10,  10, 11, 8,

        // Right face
        12, 13, 14, 14, 15, 12,

        // Top face
        16, 17, 18, 18, 19, 16,

        // Bottom face
        20, 21, 22, 22, 23, 20
    };

    std::vector<Vertex> skyboxVerts(skyboxVertices, skyboxVertices + sizeof(skyboxVertices) / sizeof(Vertex));
    std::vector<GLuint> skyboxInds(skyboxIndices, skyboxIndices + sizeof(skyboxIndices) / sizeof(GLuint));;
    std::vector<Texture> skyboxTex;
    Mesh skybox(skyboxVerts, skyboxInds, skyboxTex);

    VoxelWorld world(shader, *camera, 0);

    double curTime = 0.0;
    double prevTime = 0.0;
    double timeDiff;
    unsigned int fpsCounter = 0;

    // Disables V-Sync
    glfwSwapInterval(0);

    static int oldState = GLFW_RELEASE;

    while (!glfwWindowShouldClose(window))
    {
        curTime = glfwGetTime();
        timeDiff = curTime - prevTime;
        fpsCounter++;
        if (timeDiff >= 1.0 / 30.0) {
            std::string fps = std::to_string((1.0 / timeDiff) * fpsCounter);
            std::string ms = std::to_string((timeDiff / fpsCounter) * 1000);
            std::string newTitle = "Voxl - " + fps + "FPS / " + ms + "ms";

            glfwSetWindowTitle(window, newTitle.c_str());

            prevTime = curTime;
            fpsCounter = 0;

            camera->HandleInput(window);
        }

        glClearColor(0.0, 0.2, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
            RaycastHit outHit{};
            if (world.Raycast(camera->Position, camera->Direction, outHit)) {
                spdlog::info("Hit: {0},{1},{2}", outHit.WorldPositionHit.Position.x, outHit.WorldPositionHit.Position.y, outHit.WorldPositionHit.Position.z);
                world.DestroyVoxelBlock(outHit.WorldPositionHit);
                // world.PlaceVoxelBlock(outHit.WorldPositionHit);
            }else {
                spdlog::info("Hit nothing");
            }
        }
        oldState = newState;

        camera->UpdateMatrix(90.0f, 0.1f, 100.0f);

        // SKYBOX
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glCullFace(GL_FRONT);

        skyboxShader.Activate();
        skybox.DrawViewProj(skyboxShader, *camera);

        glCullFace(GL_BACK);
        glDepthMask(GL_TRUE);

        // WORLD
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        shader.Activate();

        // TODO: camera.Direction will need to be changed
        world.Update(curTime / 1.0f, camera->Position, camera->Direction);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader.Delete();
    skyboxShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
