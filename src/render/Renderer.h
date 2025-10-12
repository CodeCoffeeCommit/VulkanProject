#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class VulkanContext;
class SwapChain;
class GraphicsPipeline;  // <- ADD THIS

class Renderer {
public:
    Renderer();
    ~Renderer();

    void init(VulkanContext* context, SwapChain* swapChain);
    void cleanup();

    void drawFrame();
    void waitIdle();

private:
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    VulkanContext* context = nullptr;
    SwapChain* swapChain = nullptr;
    GraphicsPipeline* pipeline = nullptr;  // <- ADD THIS

    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;

    // Sync objects - one per swap chain image
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    uint32_t currentFrame = 0;
};