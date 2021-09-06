//
// Created by aeternum on 9/6/21.
//

#ifndef VULKANAPI_QUEUE_H
#define VULKANAPI_QUEUE_H

#include "Device.h"

namespace VulkanApi
{
    class Queue
    {
    public:
        Queue(Device& device, uint32_t queueFamilyIndex, uint32_t queueIndex = 0);

        inline Device& GetDevice() { return m_Device; }
        inline VkQueue& GetVkQueue() { return m_Queue; }

    private:
        Device& m_Device;
        VkQueue m_Queue;
    };
}

#endif //VULKANAPI_QUEUE_H
