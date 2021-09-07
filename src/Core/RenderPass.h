//
// Created by aeternum on 9/7/21.
//

#ifndef VULKANAPI_RENDERPASS_H
#define VULKANAPI_RENDERPASS_H

#include "Device.h"
#include "SwapChain.h"

namespace VulkanApi
{
    class RenderPass
    {
    public:
        RenderPass(Device& device, SwapChain& swapChain);

        inline VkRenderPass& GetVkRenderPass() { return m_RenderPass; }

    private:
        Device& m_Device;
        SwapChain& m_SwapChain;

        VkRenderPass m_RenderPass;
    };
}

#endif //VULKANAPI_RENDERPASS_H
