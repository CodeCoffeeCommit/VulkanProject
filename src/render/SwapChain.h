#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class VulkanContext;

class SwapChain {
public:
    SwapChain();
    ~SwapChain();

    void init(VulkanContext* context, GLFWwindow* window);
    void cleanup();

    void recreate(GLFWwindow* window);

    // Getters
    VkSwapchainKHR getSwapChain() const { return swapChain; }
    VkFormat getImageFormat() const { return swapChainImageFormat; }
    VkExtent2D getExtent() const { return swapChainExtent; }
    const std::vector<VkFramebuffer>& getFramebuffers() const { return swapChainFramebuffers; }
    VkRenderPass getRenderPass() const { return renderPass; }

    uint32_t getImageCount() const { return static_cast<uint32_t>(swapChainImages.size()); }

private:
    void createSwapChain(GLFWwindow* window);
    void createImageViews();
    void createRenderPass();
    void createFramebuffers();

    // Helper functions
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

    VulkanContext* context = nullptr;

    // Swap chain objects
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    // Render pass and framebuffers
    VkRenderPass renderPass = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> swapChainFramebuffers;
};