//
// Created by aeternum on 9/6/21.
//

#ifndef VULKANAPI_GRAPHICSPIPLINE_H
#define VULKANAPI_GRAPHICSPIPLINE_H

/*
    This file is temporary and it might change in future according to it's needs.
 */

#include "Device.h"

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

    class PipelineLayout
    {
    public:
        PipelineLayout(Device& device);

    private:
        Device& m_Device;
        VkPipelineLayout m_PipelineLayout;
    };
}

#endif //VULKANAPI_GRAPHICSPIPLINE_H
