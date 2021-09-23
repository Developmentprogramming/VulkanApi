//
// Created by aeternum on 9/9/21.
//

#include "Application.h"

#include <GLFW/glfw3.h>
#include <stdexcept>

#include "Core/RenderPass.h"
#include "Core/Queue.h"

namespace VulkanApi
{

    Application::Application()
        : Window(800, 600),
        m_Instance(CreateRef<Instance, const std::initializer_list<const char*>&>({ "VK_LAYER_KHRONOS_validation" })),
        m_Surface(CreateRef<Surface>(*this, m_Instance)),
        m_PhysicalDevice(CreateRef<PhysicalDevice>(m_Instance, m_Surface))
    {
        m_PhysicalDevice->PickSuitableDevice();

        m_Device = CreateRef<Device, const std::initializer_list<const char*>&, const Ref<PhysicalDevice>&>({ VK_KHR_SWAPCHAIN_EXTENSION_NAME }, m_PhysicalDevice);
        auto indices = m_PhysicalDevice->GetQueueFamilyProperties();

        m_GraphicsQueue = CreateRef<Queue>();
        m_GraphicsQueue->GetDeviceQueue(m_Device, indices.graphicsFamily.value());
        m_PresentQueue = CreateRef<Queue>();
        m_PresentQueue->GetDeviceQueue(m_Device, indices.presentFamily.value());

        m_SwapChain = CreateRef<SwapChain>(*this, m_Device, m_Surface);
        m_RenderPass = CreateRef<RenderPass>(m_Device, m_SwapChain);

        m_Shader = CreateRef<Shader>(m_Device, "assets/shaders-spv/shader.vert.spv", "assets/shaders-spv/shader.frag.spv");

        VkViewport viewport;
        auto& extent = m_SwapChain->GetVkExtent();
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)extent.width;
        viewport.height = (float)extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor {};
        scissor.offset = { 0, 0 };
        scissor.extent = extent;

        VkPipelineColorBlendAttachmentState colorAttachment {};
        colorAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorAttachment.blendEnable = VK_FALSE;

        VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo {};
        depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

        VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo {};
        dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

        m_GraphicsPipeline = CreateRef<Pipeline>(m_Device, m_RenderPass, m_Shader,
                                                 GraphicsPipeline::GetVertexInputStateCreateInfo(),
                                                 GraphicsPipeline::GetInputAssemblyCreateInfo(),
                                                 GraphicsPipeline::GetViewportStateCreateInfo({ viewport }, { scissor }),
                                                 GraphicsPipeline::GetRasterizationStateCreateInfo(),
                                                 GraphicsPipeline::GetMultisampleStateCreateInfo(),
                                                 GraphicsPipeline::GetColorBlendStateCreateInfo({ colorAttachment }),
                                                 depthStencilStateCreateInfo,
                                                 dynamicStateCreateInfo);
        m_SwapChain->CreateFrameBuffers(*m_RenderPass);
        m_CommandPool = CreateRef<CommandPool>(m_Device);
        m_CommandBuffers = CreateRef<CommandBuffers>(m_CommandPool, m_SwapChain, m_RenderPass, m_GraphicsPipeline);
        m_CommandBuffers->Begin();

        m_ImageAvailableSemaphore = CreateRef<Semaphore>(m_Device);
        m_RenderFinishedSemaphore = CreateRef<Semaphore>(m_Device);

        // Destroy shader modules
        m_Shader->ReleaseModules();
    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        while(!Closed())
        {
            glfwPollEvents();
            DrawFrame();
        }
    }

    void Application::DrawFrame()
    {
        uint32_t imageIndex;
        vkAcquireNextImageKHR(m_Device->GetVkDevice(), m_SwapChain->GetVkSwapChain(), UINT64_MAX, m_ImageAvailableSemaphore->GetVkSemaphore(), VK_NULL_HANDLE, &imageIndex);

        VkSubmitInfo submitInfo {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphore[] = { m_ImageAvailableSemaphore->GetVkSemaphore() };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphore;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_CommandBuffers->GetCommandBuffers()[imageIndex];

        VkSemaphore signalSemaphore[] = { m_RenderFinishedSemaphore->GetVkSemaphore() };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphore;

        if (vkQueueSubmit(m_GraphicsQueue->GetVkQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
            throw std::runtime_error("Failed to submit to graphics queue!");

        VkPresentInfoKHR presentInfoKhr {};
        presentInfoKhr.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfoKhr.waitSemaphoreCount = 1;
        presentInfoKhr.pWaitSemaphores = signalSemaphore;

        VkSwapchainKHR swapChain[] = { m_SwapChain->GetVkSwapChain() };
        presentInfoKhr.swapchainCount = 1;
        presentInfoKhr.pSwapchains = swapChain;
        presentInfoKhr.pImageIndices = &imageIndex;

        vkQueuePresentKHR(m_PresentQueue->GetVkQueue(), &presentInfoKhr);
        vkQueueWaitIdle(m_PresentQueue->GetVkQueue());
    }
}