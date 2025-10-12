#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render/VulkanContext.h"
#include "render/SwapChain.h"
#include "render/Renderer.h"

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
    SwapChain* swapChain = nullptr;
    Renderer* renderer = nullptr;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Libre DCC Tool - Day 3-4", nullptr, nullptr);

        if (!window) {
            throw std::runtime_error("Failed to create GLFW window!");
        }

        std::cout << "[OK] Window created" << std::endl;
    }

    void initVulkan() {
        // Initialize Vulkan context
        vulkanContext = new VulkanContext();
        vulkanContext->init(window);

        // Create swap chain
        swapChain = new SwapChain();
        swapChain->init(vulkanContext, window);

        // Create renderer
        renderer = new Renderer();
        renderer->init(vulkanContext, swapChain);
    }

    void mainLoop() {
        std::cout << "\n==================================" << std::endl;
        std::cout << "Rendering! You should see color!" << std::endl;
        std::cout << "Close window to exit." << std::endl;
        std::cout << "==================================\n" << std::endl;

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            renderer->drawFrame();
        }

        // Wait for operations to finish
        renderer->waitIdle();
    }

    void cleanup() {
        if (renderer) {
            renderer->cleanup();
            delete renderer;
        }

        if (swapChain) {
            swapChain->cleanup();
            delete swapChain;
        }

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
    std::cout << "LIBRE DCC TOOL - Day 3-4" << std::endl;
    std::cout << "Initializing Vulkan rendering..." << std::endl;
    std::cout << "==================================\n" << std::endl;

    Application app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "\n[ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "\n[SUCCESS] Rendering worked!" << std::endl;
    return EXIT_SUCCESS;
}