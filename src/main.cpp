#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/Window.h"
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
    Window* window = nullptr;
    VulkanContext* vulkanContext = nullptr;
    SwapChain* swapChain = nullptr;
    Renderer* renderer = nullptr;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    void initWindow() {
        window = new Window(WIDTH, HEIGHT, "Libre DCC Tool - Triangle");
        std::cout << "[OK] Window created" << std::endl;
    }

    void initVulkan() {
        // Create VulkanContext with window pointer
        vulkanContext = new VulkanContext(window);
        vulkanContext->init();  // No arguments!

        // Create swap chain
        swapChain = new SwapChain();
        swapChain->init(vulkanContext, window->getHandle());

        // Create renderer
        renderer = new Renderer();
        renderer->init(vulkanContext, swapChain);
    }

    void mainLoop() {
        std::cout << "\n==================================" << std::endl;
        std::cout << "Rendering triangle!" << std::endl;
        std::cout << "Close window to exit." << std::endl;
        std::cout << "==================================\n" << std::endl;

        while (!window->shouldClose()) {
            window->pollEvents();
            renderer->drawFrame();
        }

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
            delete window;
        }

        std::cout << "[OK] Application shutdown complete" << std::endl;
    }
};

int main() {
    std::cout << "\n==================================" << std::endl;
    std::cout << "LIBRE DCC TOOL" << std::endl;
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

    std::cout << "\n[SUCCESS] Program completed!" << std::endl;
    return EXIT_SUCCESS;
}