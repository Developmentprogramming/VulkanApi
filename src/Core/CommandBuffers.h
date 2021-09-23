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
        CommandBuffers(const Ref<CommandPool>& commandPool, const Ref<SwapChain>& swapChain, const Ref<RenderPass>& renderPass, const Ref<Pipeline>& pipeline);
        virtual ~CommandBuffers();

        void Begin() const;

        inline std::vector<VkCommandBuffer>& GetCommandBuffers() { return m_CommandBuffers; }

    private:
        Ref<CommandPool> m_CommandPool;
        Ref<SwapChain> m_SwapChain;
        Ref<RenderPass> m_RenderPass;
        Ref<Pipeline> m_Pipeline;

        std::vector<VkCommandBuffer> m_CommandBuffers;
    };
}

#endif //VULKANAPI_COMMANDBUFFERS_H
