//
// Created by Ben Gotts on 06/07/2025.
//

#include "Camera.h"

#include "Input.h"
#include "Time.h"

Camera::Camera(Window* window, int width, int height, glm::vec3 pos) : _window(window), _width(width), _height(height), Position(pos) {
    Right = glm::normalize(glm::cross(Direction, Up));
    Forward = Direction;

    glfwSetInputMode(_window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Camera::CalculateMatrix(Shader& shader, const char* uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(CameraMatrix));
    glUniform3f(glGetUniformLocation(shader.ID, "cameraPosition"), Position.x, Position.y, Position.z);
}

glm::mat4 Camera::GetView() {
    return glm::lookAt(Position, Position + Direction, Up);
}

glm::mat4 Camera::GetProjection() {
    float FOV = 90.0f;
    float nearPane = 0.01f;
    float farPane = 100.0f;

    return glm::perspective(glm::radians(FOV), (float)_width / (float)_height, nearPane, farPane);
}

void Camera::Refresh(int width, int height) {
    _width = static_cast<float>(width);
    _height = static_cast<float>(height);
}

void Camera::UpdateMatrix(float FOV, float nearPane, float farPane) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = GetView();
    projection = GetProjection();

    CameraMatrix = projection * view;
}

void Camera::HandleInput() {
    if (Input::GetKeyHold(KeyCode::W)) {
        HandleMovement(Direction);
    }
    if (Input::GetKeyHold(KeyCode::A)) {
        HandleMovement(-Right);
    }
    if (Input::GetKeyHold(KeyCode::S)) {
        HandleMovement(-Direction);
    }
    if (Input::GetKeyHold(KeyCode::D)) {
        HandleMovement(Right);
    }

    if (Input::GetKeyHold(KeyCode::SPACE)) {
        HandleMovement(Up);
    }
    if (Input::GetKeyHold(KeyCode::LEFT_CONTROL)) {
        HandleMovement(-Up);
    }

    HandleRotation();
    glfwSetCursorPos(_window->GetNativeWindow(), (_width / 2), (_height / 2));
}

void Camera::HandleMovement(glm::vec3 movementDirection) {
    Position += movementDirection * _speed * static_cast<float>(Time::DeltaTime);
}

void Camera::HandleRotation() {
    double mouseX;
    double mouseY;
    glfwGetCursorPos(_window->GetNativeWindow(), &mouseX, &mouseY);

    float rotationX = _lookSensitivity * (float)(mouseY - (_height / 2)) / _height;
    float rotationY = _lookSensitivity * (float)(mouseX - (_width / 2)) / _width;

    glm::vec3 newDirection = glm::rotate(Direction, glm::radians(-rotationX), Right);

    if (!((glm::angle(newDirection, Up) <= glm::radians(5.0f)) or (glm::angle(newDirection, -Up) <= glm::radians(5.0f)))){
        Direction = newDirection;
    }

    Direction = glm::rotate(Direction, glm::radians(-rotationY), Up);

    Right = glm::normalize(glm::cross(Direction, Up));
    Forward = Direction;
}