#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    // Prevent copying
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Window operations
    bool shouldClose() const;
    void pollEvents();
    VkExtent2D getExtent() const;
    GLFWwindow* getHandle() const { return window; }

    // Resize handling
    bool wasResized() const { return framebufferResized; }
    void resetResizeFlag() { framebufferResized = false; }

private:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window = nullptr;
    int width;
    int height;
    std::string title;
    bool framebufferResized = false;
};