//
// Created by aeternum on 9/6/21.
//

#include "SwapChain.h"

#include <GLFW/glfw3.h>
#include <functional>
#include <stdexcept>

#include "RenderPass.h"

namespace VulkanApi
{

    SwapChain::SwapChain(Window& window, Device &device, Surface &surface)
        : m_Window(window), m_Device(device), m_Surface(surface)
    {
        m_Details = QuerySupport();

        VkSurfaceFormatKHR format = ChooseFormat();
        VkPresentModeKHR mode = ChoosePresentMode();
        VkExtent2D extent = ChooseSwapExtent();

        uint32_t imageCount = m_Details.capabilities.minImageCount + 1;
        if (m_Details.capabilities.maxImageCount > 0 && imageCount > m_Details.capabilities.maxImageCount)
            imageCount = m_Details.capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_Surface.GetVkSurface();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = format.format;
        createInfo.imageColorSpace = format.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        PhysicalDevice::QueueFamilyDetails indices = m_Device.GetPhysicalDevice().GetQueueFamilyProperties();
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

        createInfo.preTransform = m_Details.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = mode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_Device.GetVkDevice(), &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
            throw std::runtime_error("Failed to create swap chain!");

        vkGetSwapchainImagesKHR(m_Device.GetVkDevice(), m_SwapChain, &imageCount, nullptr);
        m_Images.resize(imageCount);
        vkGetSwapchainImagesKHR(m_Device.GetVkDevice(), m_SwapChain, &imageCount, m_Images.data());

        m_Format = format.format;
        m_Extent = extent;

        m_ImageViews.resize(m_Images.size());

        for (size_t i = 0; i < m_ImageViews.size(); i++)
        {
            VkImageViewCreateInfo imageViewCreateInfo {};
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.image = m_Images[i];
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = m_Format;
            imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_Device.GetVkDevice(), &imageViewCreateInfo, nullptr, &m_ImageViews[i]) != VK_SUCCESS)
                throw std::runtime_error("Failed to create image view!");
        }
    }

    void SwapChain::CreateFrameBuffers(RenderPass &renderPass)
    {
        m_FrameBuffers.resize(m_ImageViews.size());

        for (size_t i = 0; i < m_FrameBuffers.size(); i++)
        {
            VkImageView attachments[] = { m_ImageViews[i] };

            VkFramebufferCreateInfo framebufferCreateInfo {};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = renderPass.GetVkRenderPass();
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.pAttachments = attachments;
            framebufferCreateInfo.width = m_Extent.width;
            framebufferCreateInfo.height = m_Extent.height;
            framebufferCreateInfo.layers = 1;

            if (vkCreateFramebuffer(m_Device.GetVkDevice(), &framebufferCreateInfo, nullptr, &m_FrameBuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("Failed to create frame buffer!");
        }
    }

    SwapChain::SupportDetails SwapChain::QuerySupport()
    {
        SupportDetails details;
        auto physicalDevice = m_Device.GetPhysicalDevice().GetVkPhysicalDevice();
        auto surface = m_Surface.GetVkSurface();

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR SwapChain::ChooseFormat() const
    {
        return ChooseFormat(m_Details.formats);
    }

    VkPresentModeKHR SwapChain::ChoosePresentMode() const
    {
        return ChoosePresentMode(m_Details.presentModes);
    }

    VkExtent2D SwapChain::ChooseSwapExtent() const
    {
        return ChooseSwapExtent(m_Details.capabilities, m_Window);
    }

    VkSurfaceFormatKHR SwapChain::ChooseFormat(const std::vector<VkSurfaceFormatKHR> &formats)
    {
        for (const auto& format : formats)
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return format;

        return formats[0];
    }

    VkPresentModeKHR SwapChain::ChoosePresentMode(const std::vector<VkPresentModeKHR> &modes)
    {
        for (const auto& mode : modes)
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
                return mode;

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, Window& window)
    {
        if (capabilities.currentExtent.width != UINT32_MAX)
            return capabilities.currentExtent;

        int width, height;
        glfwGetFramebufferSize(window.GetWindow(), &width, &height);

        VkExtent2D actualExtent = { (uint32_t)width, (uint32_t)height };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
