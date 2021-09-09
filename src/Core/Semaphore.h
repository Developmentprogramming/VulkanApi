//
// Created by aeternum on 9/9/21.
//

#ifndef VULKANAPI_SEMAPHORE_H
#define VULKANAPI_SEMAPHORE_H

#include "Device.h"

namespace VulkanApi
{
    class Semaphore
    {
    public:
        Semaphore(Device& device);

        inline VkSemaphore& GetVkSemaphore() { return m_Semaphore; }

    private:
        Device& m_Device;

        VkSemaphore m_Semaphore;
    };
}

#endif //VULKANAPI_SEMAPHORE_H
