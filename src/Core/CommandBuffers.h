//
// Created by aeternum on 9/9/21.
//

#ifndef VULKANAPI_COMMANDBUFFERS_H
#define VULKANAPI_COMMANDBUFFERS_H

#include "CommandPool.h"
#include "SwapChain.h"
#include "RenderPass.h"
#include "GraphicsPipline.h"

namespace VulkanApi
{
    class CommandBuffers
    {
    public:
        CommandBuffers(CommandPool& commandPool, SwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline);

        void Begin() const;

        inline std::vector<VkCommandBuffer>& GetCommandBuffers() { return m_CommandBuffers; }

    private:
        CommandPool& m_CommandPool;
        SwapChain& m_SwapChain;
        RenderPass& m_RenderPass;
        Pipeline& m_Pipeline;

        std::vector<VkCommandBuffer> m_CommandBuffers;
    };
}

#endif //VULKANAPI_COMMANDBUFFERS_H
