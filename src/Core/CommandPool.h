//
// Created by aeternum on 9/9/21.
//

#ifndef VULKANAPI_COMMANDPOOL_H
#define VULKANAPI_COMMANDPOOL_H

#include "Device.h"

namespace VulkanApi
{
    class CommandPool
    {
    public:
        CommandPool(Device& device);

    private:
        Device& m_Device;

        VkCommandPool m_CommandPool;
    };
}

#endif //VULKANAPI_COMMANDPOOL_H
