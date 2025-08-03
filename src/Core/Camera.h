//
// Created by Ben Gotts on 06/07/2025.
//

#ifndef CAMERA_H
#define CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "../Rendering/Shader.h"

class Camera {
public:
    Camera(int width, int height, glm::vec3 pos);

    void Refresh(int width, int height);
    // Sends matrix data to the vertex shader
    void CalculateMatrix(Shader& shader, const char* uniform);
    // Updates the cameras matrix
    void UpdateMatrix(float FOV, float nearPane, float farPane);
    glm::mat4 GetView();
    glm::mat4 GetProjection();

    void HandleInput(GLFWwindow* window);

    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Direction = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::mat4 CameraMatrix = glm::mat4(1);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 Right;
    glm::vec3 Forward;
private:
    float _width;
    float _height;
    float _speed = 2.0f;
    float _lookSensitivity = 100.0f;
    bool _firstClick;
};



#endif //CAMERA_H
