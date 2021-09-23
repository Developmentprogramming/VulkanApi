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
        Queue() = default;
        inline void GetDeviceQueue(const Ref<Device>& device, uint32_t queueFamilyIndex, uint32_t queueIndex = 0) { vkGetDeviceQueue(device->GetVkDevice(), queueFamilyIndex, queueIndex, &m_Queue); };

        inline VkQueue& GetVkQueue() { return m_Queue; }

    private:
        VkQueue m_Queue;
    };
}

#endif //VULKANAPI_QUEUE_H
