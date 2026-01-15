#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class VulkanContext;
class SwapChain;
class GraphicsPipeline;
class UniformBuffer;
class Grid;
class Mesh;
class Camera;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void init(VulkanContext* context, SwapChain* swapChain);
    void cleanup();

    void drawFrame(Camera* camera);
    void waitIdle();

    // Scene objects
    Grid* getGrid() { return grid; }
    Mesh* getCube() { return cube; }

private:
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();
 
    void createSceneObjects();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void updateUniformBuffer(uint32_t currentImage, Camera* camera);

    // Helper functions
    
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VulkanContext* context = nullptr;
    SwapChain* swapChain = nullptr;
    GraphicsPipeline* pipeline = nullptr;
    UniformBuffer* uniformBuffer = nullptr;

    // Scene objects
    Grid* grid = nullptr;
    Mesh* cube = nullptr;

    
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    uint32_t currentFrame = 0;
};