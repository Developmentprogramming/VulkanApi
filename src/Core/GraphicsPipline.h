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
    class GraphicsPipeline
    {
    public:
        static VkPipelineVertexInputStateCreateInfo GetVertexInputStateCreateInfo(const std::vector<VkVertexInputBindingDescription>& vertexBindings, const std::vector<VkVertexInputAttributeDescription>& attributeDescriptions);
        static VkPipelineInputAssemblyStateCreateInfo GetInputAssemblyCreateInfo();
        static VkPipelineViewportStateCreateInfo GetViewportStateCreateInfo(const std::vector<VkViewport>& viewports, const std::vector<VkRect2D>& scissors);
        static VkPipelineRasterizationStateCreateInfo GetRasterizationStateCreateInfo();
        static VkPipelineMultisampleStateCreateInfo GetMultisampleStateCreateInfo();
        static VkPipelineColorBlendStateCreateInfo GetColorBlendStateCreateInfo(const std::vector<VkPipelineColorBlendAttachmentState>& colorAttachments);

    private:
        GraphicsPipeline() = default;
    };

    class Pipeline
    {
    public:
        Pipeline(const Ref<Device>& device, const Ref<RenderPass>& renderPass, const Ref<Shader>& shader,
                 VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo,
                 VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo,
                 VkPipelineViewportStateCreateInfo viewportStateCreateInfo,
                 VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo,
                 VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo,
                 VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo,
                 VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo,
                 VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo);
        virtual ~Pipeline();

        inline VkPipelineLayout& GetVkPipelineLayout() { return m_PipelineLayout; }
        inline VkPipeline& GetVkPipeline() { return m_Pipeline; }

    private:
        Ref<Device> m_Device;
        Ref<RenderPass> m_RenderPass;
        Ref<Shader> m_Shader;

        VkPipelineLayout m_PipelineLayout;
        VkPipeline m_Pipeline;
    };
}

#endif //VULKANAPI_GRAPHICSPIPLINE_H
