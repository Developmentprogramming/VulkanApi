//
// Created by aeternum on 9/5/21.
//

#include <stdexcept>
#include <set>
#include "Device.h"

namespace VulkanApi
{

    PhysicalDevice::PhysicalDevice(Instance &instance, Surface& surface)
        : m_Instance(instance), m_Surface(surface), m_PhysicalDevice(VK_NULL_HANDLE)
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

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, m_Surface.GetVkSurface(), &presentSupport);

            if (presentSupport)
                indices.presentFamily = i;

            if (indices.IsComplete())
                break;
        }

        return indices;
    }

    PhysicalDevice::QueueFamilyDetails PhysicalDevice::GetQueueFamilyProperties()
    {
        return GetQueueFamilyProperties(m_PhysicalDevice);
    }

    // ==================================================================================
    // Logical Device ===================================================================
    // ==================================================================================

    Device::Device(const std::initializer_list<const char*>& deviceExtensions, PhysicalDevice &device)
        : m_DeviceExtensions(deviceExtensions), m_PhysicalDevice(device)
    {
        PhysicalDevice::QueueFamilyDetails indices = m_PhysicalDevice.GetQueueFamilyProperties();

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            auto createInfo = queueCreateInfos.emplace_back();
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            createInfo.queueFamilyIndex = queueFamily;
            createInfo.queueCount = 1;
            createInfo.pQueuePriorities = &queuePriority;
        }

        VkPhysicalDeviceFeatures deviceFeatures {};

        VkDeviceCreateInfo deviceCreateInfo {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount = (uint32_t)m_DeviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

        if (m_PhysicalDevice.GetInstance().GetValidationLayersEnabled())
        {
            auto& validationLayers = m_PhysicalDevice.GetInstance().GetValidationLayers();
            deviceCreateInfo.enabledLayerCount = (uint32_t)validationLayers.size();
            deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
            deviceCreateInfo.enabledLayerCount = 0;

        if (vkCreateDevice(m_PhysicalDevice.GetVkPhysicalDevice(), &deviceCreateInfo, nullptr, &m_Device) != VK_SUCCESS)
            throw std::runtime_error("Failed to create logical device!");
    }
}
