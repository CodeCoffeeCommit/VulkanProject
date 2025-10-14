#include "Window.h"
#include <stdexcept>
#include <iostream>

Window::Window(int width, int height, const std::string& title)
    : width(width), height(height), title(title) {

    // Initialize GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    // Configure GLFW for Vulkan
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Create window
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window!");
    }

    // Set user pointer for callbacks
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    std::cout << "[OK] Window created (" << width << "x" << height << ")" << std::endl;
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "[OK] Window destroyed" << std::endl;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
    app->width = width;
    app->height = height;
}