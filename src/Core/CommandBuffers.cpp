//
// Created by aeternum on 9/9/21.
//

#include "CommandBuffers.h"

#include <stdexcept>

namespace VulkanApi
{

    CommandBuffers::CommandBuffers(CommandPool &commandPool, SwapChain &swapChain, RenderPass &renderPass, Pipeline& pipeline)
        : m_CommandPool(commandPool), m_SwapChain(swapChain), m_RenderPass(renderPass), m_Pipeline(pipeline),
        m_CommandBuffers(swapChain.GetFrameBuffers().size())
    {
        VkCommandBufferAllocateInfo allocateInfo {};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool = m_CommandPool.GetVkCommandPool();
        allocateInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

        if (vkAllocateCommandBuffers(m_CommandPool.GetDevice().GetVkDevice(), &allocateInfo, m_CommandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate command buffers!");
    }

    void CommandBuffers::Begin() const
    {
        for (size_t i = 0; i < m_CommandBuffers.size(); i++)
        {
            VkCommandBufferBeginInfo beginInfo {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = 0;
            beginInfo.pInheritanceInfo = nullptr;

            if (vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo) != VK_SUCCESS)
                throw std::runtime_error("Failed to begin recording command buffer!");

            VkRenderPassBeginInfo renderPassBeginInfo {};
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.renderPass = m_RenderPass.GetVkRenderPass();
            renderPassBeginInfo.framebuffer = m_SwapChain.GetFrameBuffers()[i];
            renderPassBeginInfo.renderArea.offset = { 0, 0 };
            renderPassBeginInfo.renderArea.extent = m_SwapChain.GetVkExtent();

            VkClearValue colorValue = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues = &colorValue;

            vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline.GetVkPipeline());
            vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(m_CommandBuffers[i]);

            if (vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("Failed to record command buffers!");
        }
    }
}
