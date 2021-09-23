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
        RenderPass(const Ref<Device>& device, const Ref<SwapChain>& swapChain);
        virtual ~RenderPass();


        inline VkRenderPass& GetVkRenderPass() { return m_RenderPass; }

    private:
        Ref<Device> m_Device;
        Ref<SwapChain> m_SwapChain;

        VkRenderPass m_RenderPass;
    };
}

#endif //VULKANAPI_RENDERPASS_H
