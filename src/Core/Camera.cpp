//
// Created by Ben Gotts on 06/07/2025.
//

#include "Camera.h"

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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Position += _speed * Direction * (float)Time::DeltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Position += _speed * -Right * (float)Time::DeltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Position += _speed * -Direction * (float)Time::DeltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Position += _speed * Right * (float)Time::DeltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Position += _speed * Up * (float)Time::DeltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        Position += _speed * -Up * (float)Time::DeltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        _speed = 6.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
       _speed = 2.0f;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
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
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        _firstClick = true;
    }
}