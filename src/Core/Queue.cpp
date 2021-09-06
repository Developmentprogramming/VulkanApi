//
// Created by aeternum on 9/6/21.
//

#include "Queue.h"

namespace VulkanApi
{

    Queue::Queue(Device &device, uint32_t queueFamilyIndex, uint32_t queueIndex)
        : m_Device(device)
    {
        vkGetDeviceQueue(m_Device.GetVkDevice(), queueFamilyIndex, queueIndex, &m_Queue);
    }
}