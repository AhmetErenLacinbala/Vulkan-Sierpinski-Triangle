#pragma once

#include "lve_device.hpp"
#include <string>
#include <vector>

namespace lve
{

    struct PipelineConfigInfo
    {
        PipelineConfigInfo(const PipelineConfigInfo &) = delete;   
        PipelineConfigInfo& operator=(const PipelineConfigInfo &) = delete; 

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class LvePipeline
    {
    public:
        LvePipeline(LveDevice &device, const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);

        ~LvePipeline();
        LvePipeline(const LvePipeline &) = delete;    // not copyable
        LvePipeline& operator=(const LvePipeline &) = delete; // not copyable


        void bind(VkCommandBuffer commandBuffer);
        static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

    private:
        static std::vector<char> readFile(const std::string &filepath);
        void createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);
        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        LveDevice &lveDevice;            // reference to logical device which the pipeline will be used with
        VkPipeline graphicsPipeline;     // handle to graphics pipeline object
        VkShaderModule vertShaderModule; // handle to vertex shader module object
        VkShaderModule fragShaderModule; // handle to fragment shader module object
        // actually all 3 above are the pointers to the objects in the GPU memory
    };
} // namespace lve
