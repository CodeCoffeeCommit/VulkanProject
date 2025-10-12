#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render/VulkanContext.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class Application {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window = nullptr;
    VulkanContext* vulkanContext = nullptr;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Libre DCC Tool - Day 1", nullptr, nullptr);

        if (!window) {
            throw std::runtime_error("Failed to create GLFW window!");
        }

        std::cout << "[OK] Window created" << std::endl;
    }

    void initVulkan() {
        vulkanContext = new VulkanContext();
        vulkanContext->init(window);
    }

    void mainLoop() {
        std::cout << "\n==================================" << std::endl;
        std::cout << "Window is running. Close it to exit." << std::endl;
        std::cout << "==================================\n" << std::endl;

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        if (vulkanContext) {
            vulkanContext->cleanup();
            delete vulkanContext;
        }

        if (window) {
            glfwDestroyWindow(window);
        }

        glfwTerminate();
        std::cout << "[OK] Application shutdown complete" << std::endl;
    }
};

int main() {
    std::cout << "\n==================================" << std::endl;
    std::cout << "LIBRE DCC TOOL - Day 1" << std::endl;
    std::cout << "Initializing Vulkan..." << std::endl;
    std::cout << "==================================\n" << std::endl;

    Application app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "\n[ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "\n[SUCCESS] Vulkan is working!" << std::endl;
    return EXIT_SUCCESS;
}