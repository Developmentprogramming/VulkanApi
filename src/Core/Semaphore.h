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
        Semaphore(const Ref<Device>& device);
        virtual ~Semaphore();

        inline VkSemaphore& GetVkSemaphore() { return m_Semaphore; }

    private:
        Ref<Device> m_Device;

        VkSemaphore m_Semaphore;
    };
}

#endif //VULKANAPI_SEMAPHORE_H
