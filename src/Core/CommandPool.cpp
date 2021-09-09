//
// Created by aeternum on 9/9/21.
//

#include <stdexcept>
#include "CommandPool.h"

namespace VulkanApi
{

    CommandPool::CommandPool(Device &device)
        : m_Device(device)
    {
        auto indices = m_Device.GetPhysicalDevice().GetQueueFamilyProperties();

        VkCommandPoolCreateInfo commandPoolCreateInfo {};
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        commandPoolCreateInfo.flags = 0;

        if (vkCreateCommandPool(m_Device.GetVkDevice(), &commandPoolCreateInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
            throw std::runtime_error("Failed to create command pool!");
    }
}