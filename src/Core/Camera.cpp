//
// Created by Ben Gotts on 06/07/2025.
//

#include "Camera.h"

#include "Input.h"
#include "Time.h"

Camera::Camera(int width, int height, glm::vec3 pos) : _width(width), _height(height), Position(pos) {
    Right = glm::normalize(glm::cross(Direction, Up));
    Forward = Direction;
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

void Camera::HandleInput(GLFWwindow *window) {
    if (Input::GetKeyHold(KeyCode::W)) {
        Position += _speed * Direction * (float)Time::DeltaTime;
    }
    if (Input::GetKeyHold(KeyCode::A)) {
        Position += _speed * -Right * (float)Time::DeltaTime;
    }
    if (Input::GetKeyHold(KeyCode::S)) {
        Position += _speed * -Direction * (float)Time::DeltaTime;
    }
    if (Input::GetKeyHold(KeyCode::D)) {
        Position += _speed * Right * (float)Time::DeltaTime;
    }

    if (Input::GetKeyHold(KeyCode::SPACE)) {
        Position += _speed * Up * (float)Time::DeltaTime;
    }
    if (Input::GetKeyHold(KeyCode::LEFT_CONTROL)) {
        Position += _speed * -Up * (float)Time::DeltaTime;
    }

    if (Input::GetMouseButtonHold(MouseButton::RIGHT)) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (_firstClick) {
            glfwSetCursorPos(window, (_width / 2), (_height / 2));
            _firstClick = false;
        }

        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotationX = _lookSensitivity * (float)(mouseY - (_height / 2)) / _height;
        float rotationY = _lookSensitivity * (float)(mouseX - (_width / 2)) / _width;

        glm::vec3 newDirection = glm::rotate(Direction, glm::radians(-rotationX), Right);

        if (!((glm::angle(newDirection, Up) <= glm::radians(5.0f)) or (glm::angle(newDirection, -Up) <= glm::radians(5.0f)))){
            Direction = newDirection;
        }

        Direction = glm::rotate(Direction, glm::radians(-rotationY), Up);

        Right = glm::normalize(glm::cross(Direction, Up));
        Forward = Direction;

        glfwSetCursorPos(window, (_width / 2), (_height / 2));
    }

    if (Input::GetMouseButtonUp(MouseButton::RIGHT)) {
        _firstClick = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}