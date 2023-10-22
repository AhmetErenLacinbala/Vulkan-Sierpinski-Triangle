#pragma once

#include "lve_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


#include <vector>

namespace lve
{
    class LveModel
    {
    public:

    struct Vertex
    {
        glm::vec2 position;
        glm::vec3 color;
        static std::vector<VkVertexInputBindingDescription> getBindingDescription();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
    };
    
        LveModel(LveDevice &device, const std::vector<Vertex> &vertices);
        ~LveModel();
        LveModel(const LveModel &) = delete;
        LveModel &operator=(const LveModel &) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);
    private:

        void createVertexBuffers(const std::vector<Vertex> &vertices);
        LveDevice &lveDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}