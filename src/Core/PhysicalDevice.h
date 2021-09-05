//
// Created by aeternum on 9/5/21.
//

#ifndef VULKANAPI_PHYSICALDEVICE_H
#define VULKANAPI_PHYSICALDEVICE_H

#include <vulkan/vulkan.h>
#include <optional>

#include "Instance.h"

namespace VulkanApi
{
    class PhysicalDevice
    {
    public:
        struct QueueFamilyDetails
        {
            std::optional<uint32_t> graphicsFamily;
            inline bool IsComplete() const { return graphicsFamily.has_value(); }
        };

    public:
        PhysicalDevice(Instance& instance);
        void PickSuitableDevice();

        static QueueFamilyDetails GetQueueFamilyProperties(VkPhysicalDevice device);

    private:
        static bool IsDeviceSuitable(VkPhysicalDevice device);

    private:
        Instance& m_Instance;

        VkPhysicalDevice m_PhysicalDevice;
        uint32_t m_DeviceCount;
        std::vector<VkPhysicalDevice> m_Devices;
    };
}


#endif //VULKANAPI_PHYSICALDEVICE_H
