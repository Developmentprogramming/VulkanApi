//
// Created by aeternum on 9/23/21.
//

#include <stdexcept>
#include "Fence.h"

namespace VulkanApi
{

    Fence::Fence(const Ref<Device> &device, bool initialize)
        : m_Device(device), m_Fence(VK_NULL_HANDLE), m_Initialized(initialize)
    {
        if (initialize)
        {
            VkFenceCreateInfo fenceCreateInfo{};
            fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            if (vkCreateFence(m_Device->GetVkDevice(), &fenceCreateInfo, nullptr, &m_Fence) != VK_SUCCESS)
                throw std::runtime_error("Failed to create fence!");
        }
    }

    Fence& Fence::operator=(const Fence &fence)
    {
        m_Initialized = false;
        m_Fence = fence.m_Fence;
        m_Device = fence.m_Device;

        return *this;
    }

    Fence::~Fence()
    {
        if (m_Initialized)
            vkDestroyFence(m_Device->GetVkDevice(), m_Fence, nullptr);
    }
}