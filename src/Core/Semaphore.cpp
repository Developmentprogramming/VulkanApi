//
// Created by aeternum on 9/9/21.
//

#include "Semaphore.h"

#include <stdexcept>

namespace VulkanApi
{

    Semaphore::Semaphore(Device &device)
        : m_Device(device)
    {
        VkSemaphoreCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(m_Device.GetVkDevice(), &createInfo, nullptr, &m_Semaphore) != VK_SUCCESS)
            throw std::runtime_error("Failed to create semaphore!");
    }
}