#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

class VulkanContext;
class SwapChain;

class GraphicsPipeline {
public:
    GraphicsPipeline();
    ~GraphicsPipeline();

    void init(VulkanContext* context, SwapChain* swapChain);
    void cleanup();

    VkPipeline getPipeline() const { return graphicsPipeline; }
    VkPipelineLayout getLayout() const { return pipelineLayout; }

private:
    void createGraphicsPipeline();
    VkShaderModule createShaderModule(const std::vector<char>& code);
    std::vector<char> readFile(const std::string& filename);

    VulkanContext* context = nullptr;
    SwapChain* swapChain = nullptr;

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
};