//
// Created by aeternum on 9/6/21.
//

#ifndef VULKANAPI_GRAPHICSPIPLINE_H
#define VULKANAPI_GRAPHICSPIPLINE_H

/*
    This file is temporary and it might change in future according to it's needs.
 */

#include "Device.h"
#include "RenderPass.h"
#include "ShaderModule.h"

namespace VulkanApi
{
    class GraphicsPipeline // Graphics Pipeline is a SingleTon
    {
    public:
        GraphicsPipeline(const GraphicsPipeline&) = delete;
        GraphicsPipeline(GraphicsPipeline&&) = delete;
        GraphicsPipeline& operator=(GraphicsPipeline&) = delete;
        GraphicsPipeline& operator=(GraphicsPipeline&&) = delete;

        inline static GraphicsPipeline& Get() { static GraphicsPipeline instance; return instance; }

        VkPipelineVertexInputStateCreateInfo GetVertexInputStateCreateInfo();
        VkPipelineInputAssemblyStateCreateInfo GetInputAssemblyCreateInfo();
        VkPipelineViewportStateCreateInfo GetViewportStateCreateInfo(const std::vector<VkViewport>& viewports, const std::vector<VkRect2D>& scissors);
        VkPipelineRasterizationStateCreateInfo GetRasterizationStateCreateInfo();
        VkPipelineMultisampleStateCreateInfo GetMultisampleStateCreateInfo();
        VkPipelineColorBlendStateCreateInfo GetColorBlendStateCreateInfo(const std::vector<VkPipelineColorBlendAttachmentState>& colorAttachments);

    private:
        GraphicsPipeline();
    };

    class Pipeline
    {
    public:
        Pipeline(Device& device, RenderPass& renderPass, Shader& shader,
                 VkPipelineVertexInputStateCreateInfo& vertexInputStateCreateInfo,
                 VkPipelineInputAssemblyStateCreateInfo& inputAssemblyStateCreateInfo,
                 VkPipelineViewportStateCreateInfo& viewportStateCreateInfo,
                 VkPipelineRasterizationStateCreateInfo& rasterizationStateCreateInfo,
                 VkPipelineMultisampleStateCreateInfo& multisampleStateCreateInfo,
                 VkPipelineColorBlendStateCreateInfo& colorBlendStateCreateInfo,
                 VkPipelineDepthStencilStateCreateInfo& depthStencilStateCreateInfo,
                 VkPipelineDynamicStateCreateInfo& dynamicStateCreateInfo);

        inline VkPipelineLayout& GetVkPipelineLayout() { return m_PipelineLayout; }
        inline VkPipeline& GetVkPipeline() { return m_Pipeline; }

    private:
        Device& m_Device;
        RenderPass& m_RenderPass;
        Shader& m_Shader;

        VkPipelineLayout m_PipelineLayout;
        VkPipeline m_Pipeline;
    };
}

#endif //VULKANAPI_GRAPHICSPIPLINE_H
