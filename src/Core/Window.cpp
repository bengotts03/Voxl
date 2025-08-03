//
// Created by Ben Gotts on 03/08/2025.
//

#include "Window.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    // camera->Refresh(width, height);
}

Window::Window(unsigned int width, unsigned int height) : _width(width), _height(height) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    _window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), "Voxl", nullptr, nullptr);

    if (_window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return;
    }

    glfwMakeContextCurrent(_window);
    gladLoadGL();

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(_window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Disables V-Sync
    glfwSwapInterval(0);
}

Window::~Window() {
    glfwDestroyWindow(_window);
    glfwTerminate();
}

GLFWwindow* Window::GetNativeWindow() const {
    return _window;
}

unsigned int Window::GetWindowWidth() const {
    return _width;
}

unsigned int Window::GetWindowHeight() const {
    return _height;
}