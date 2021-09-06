//
// Created by aeternum on 9/6/21.
//

#include <stdexcept>
#include "GraphicsPipline.h"

namespace VulkanApi
{

    GraphicsPipeline::GraphicsPipeline()
    {
    }

    VkPipelineVertexInputStateCreateInfo GraphicsPipeline::GetVertexInputStateCreateInfo()
    {
        VkPipelineVertexInputStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        createInfo.vertexAttributeDescriptionCount = 0;
        createInfo.pVertexAttributeDescriptions = nullptr;
        createInfo.vertexBindingDescriptionCount = 0;
        createInfo.pVertexBindingDescriptions = nullptr;

        return createInfo;
    }

    VkPipelineInputAssemblyStateCreateInfo GraphicsPipeline::GetInputAssemblyCreateInfo()
    {
        VkPipelineInputAssemblyStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        createInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        createInfo.primitiveRestartEnable = VK_FALSE;
        return createInfo;
    }

    VkPipelineViewportStateCreateInfo GraphicsPipeline::GetViewportStateCreateInfo(const std::vector<VkViewport> &viewports, const std::vector<VkRect2D> &scissors)
    {
        VkPipelineViewportStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        createInfo.viewportCount = (uint32_t)viewports.size();
        createInfo.pViewports = viewports.data();
        createInfo.scissorCount = (uint32_t)scissors.size();
        createInfo.pScissors = scissors.data();
        return createInfo;
    }

    VkPipelineRasterizationStateCreateInfo GraphicsPipeline::GetRasterizationStateCreateInfo()
    {
        VkPipelineRasterizationStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        createInfo.depthClampEnable = VK_FALSE;
        createInfo.rasterizerDiscardEnable = VK_FALSE;
        createInfo.polygonMode = VK_POLYGON_MODE_FILL;
        createInfo.lineWidth = 1.0f;
        createInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        createInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        createInfo.depthBiasEnable = VK_FALSE;
        return createInfo;
    }

    VkPipelineMultisampleStateCreateInfo GraphicsPipeline::GetMultisampleStateCreateInfo()
    {
        VkPipelineMultisampleStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        createInfo.sampleShadingEnable = VK_FALSE;
        createInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        return createInfo;
    }

    VkPipelineColorBlendStateCreateInfo GraphicsPipeline::GetColorBlendStateCreateInfo(const std::vector<VkPipelineColorBlendAttachmentState> &colorAttachments)
    {
        VkPipelineColorBlendStateCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        createInfo.logicOpEnable = VK_FALSE;
        createInfo.logicOp = VK_LOGIC_OP_COPY;
        createInfo.attachmentCount = (uint32_t)colorAttachments.size();
        createInfo.pAttachments = colorAttachments.data();
        return createInfo;
    }

    // ================================================================
    // Pipeline Layout ================================================
    // ================================================================

    PipelineLayout::PipelineLayout(Device &device)
        : m_Device(device)
    {
        VkPipelineLayoutCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        if (vkCreatePipelineLayout(m_Device.GetVkDevice(), &createInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create pipeline layout!");
    }
}
