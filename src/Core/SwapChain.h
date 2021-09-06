//
// Created by aeternum on 9/6/21.
//

#ifndef VULKANAPI_SWAPCHAIN_H
#define VULKANAPI_SWAPCHAIN_H

#include "Surface.h"
#include "Device.h"

namespace VulkanApi
{
    class SwapChain
    {
    public:
        struct SupportDetails
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

    public:
        SwapChain(Window& window, Device& device, Surface& surface);

        inline VkSwapchainKHR& GetVkSwapChain() { return m_SwapChain; }
        inline VkFormat& GetVkFormat() { return m_Format; }
        inline VkExtent2D& GetVkExtent() { return m_Extent; }

        inline SupportDetails GetSupportDetails() { return m_Details; }
        inline std::vector<VkImage>& GetImages() { return m_Images; }
        inline std::vector<VkImageView>& GetImageViews() { return m_ImageViews; }

    private:
        SupportDetails QuerySupport();

        VkSurfaceFormatKHR ChooseFormat() const;
        VkPresentModeKHR ChoosePresentMode() const;
        VkExtent2D ChooseSwapExtent() const;

        static VkSurfaceFormatKHR ChooseFormat(const std::vector<VkSurfaceFormatKHR>& formats);
        static VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& modes);
        static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window& window);

    private:
        Device& m_Device;
        Surface& m_Surface;
        Window& m_Window;

        VkSwapchainKHR m_SwapChain;
        SupportDetails m_Details;
        std::vector<VkImage> m_Images;
        VkFormat m_Format;
        VkExtent2D m_Extent;
        std::vector<VkImageView> m_ImageViews;

    };
}

#endif //VULKANAPI_SWAPCHAIN_H
