//
// Created by aeternum on 9/5/21.
//

#ifndef VULKANAPI_INSTANCE_H
#define VULKANAPI_INSTANCE_H

#include <vulkan/vulkan.h>

#include <vector>

namespace VulkanApi
{
    class Instance
    {
    public:
        Instance(const std::initializer_list<const char*>& validationLayers);
        virtual ~Instance();

        inline VkInstance& GetInstance() { return m_Instance; }
        inline VkDebugUtilsMessengerEXT& GetDebugMessenger() { return m_DebugMessenger; }

    private:
        bool CheckValidationLayerSupport() noexcept;
        std::vector<const char*> GetRequiredExtension() const noexcept;

        static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    private:
        std::vector<const char*> m_ValidationLayers;
        std::vector<VkLayerProperties> m_AvailableLayers;

        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;

#ifdef VULKAN_API_DEBUG
        bool m_EnableValidationLayers = true;
#else
        bool m_EnableValidationLayers = false;
#endif
    };
}


#endif //VULKANAPI_INSTANCE_H
