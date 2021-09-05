//
// Created by aeternum on 9/5/21.
//

#include <stdexcept>
#include "PhysicalDevice.h"

namespace VulkanApi
{

    PhysicalDevice::PhysicalDevice(Instance &instance)
        : m_Instance(instance), m_PhysicalDevice(VK_NULL_HANDLE)
    {
        vkEnumeratePhysicalDevices(m_Instance.GetVkInstance(), &m_DeviceCount, nullptr);
        if (m_DeviceCount == 0)
            throw std::runtime_error("GPU with Vulkan support not found");

        m_Devices.resize(m_DeviceCount);
        vkEnumeratePhysicalDevices(m_Instance.GetVkInstance(), &m_DeviceCount, m_Devices.data());
    }

    void PhysicalDevice::PickSuitableDevice()
    {
        for (const auto& device : m_Devices)
        {
            if (IsDeviceSuitable(device))
            {
                m_PhysicalDevice = device;
                break;
            }
        }

        if (m_PhysicalDevice == VK_NULL_HANDLE)
            throw std::runtime_error("Failed to find suitable GPU!");
    }

    bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyDetails indices = GetQueueFamilyProperties(device);
        return indices.IsComplete();
    }

    PhysicalDevice::QueueFamilyDetails PhysicalDevice::GetQueueFamilyProperties(VkPhysicalDevice device)
    {
        QueueFamilyDetails indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;

            if (indices.IsComplete())
                break;
        }

        return indices;
    }

}
