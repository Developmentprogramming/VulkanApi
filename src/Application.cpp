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
#define MAX_FRAMES 2

    struct Vertex
    {
        glm::vec3 Position;

        static VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription {};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static VkVertexInputAttributeDescription GetAttributeDescription()
        {
            VkVertexInputAttributeDescription attributeDescription {};
            attributeDescription.binding = 0;
            attributeDescription.location = 0;
            attributeDescription.offset = offsetof(Vertex, Position);
            attributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;

            return attributeDescription;
        }
    };

    static std::vector<Vertex> vertices = {
            { {  0.0f, -0.5f, 0.0f } },
            { {  0.5f,  0.5f, 0.0f } },
            { { -0.5f,  0.5f, 0.0f } }
    };

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

        m_ImageAvailableSemaphore.reserve(MAX_FRAMES);
        m_RenderFinishedSemaphore.reserve(MAX_FRAMES);
        m_InFlightFences.reserve(MAX_FRAMES);

        for (uint32_t i = 0; i < MAX_FRAMES; i++)
        {
            m_ImageAvailableSemaphore.emplace_back(m_Device);
            m_RenderFinishedSemaphore.emplace_back(m_Device);
            m_InFlightFences.emplace_back(m_Device);
        }

        CreateSwapChain();
        m_ImagesInFlight.reserve(m_SwapChain->GetImages().size());
        for (uint32_t i = 0; i < m_SwapChain->GetImages().size(); i++)
            m_ImagesInFlight.emplace_back(m_Device, false);
    }

    void Application::CreateSwapChain()
    {
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
                                                 GraphicsPipeline::GetVertexInputStateCreateInfo({ Vertex::GetBindingDescription() }, { Vertex::GetAttributeDescription() }),
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
        if (!m_BuffersInitialized)
        {
            uint64_t bufferSize = sizeof(vertices[0]) * vertices.size();
            m_VertexBuffer = CreateRef<VertexBuffer>(m_Device, m_CommandBuffers, m_GraphicsQueue, vertices.data(), bufferSize);
            m_BuffersInitialized = true;
        }
        m_CommandBuffers->Begin({ m_VertexBuffer });

        // Destroy shader modules
        m_Shader->ReleaseModules();
    }

    void Application::RecreateSwapChain()
    {
        int32_t width = 0, height = 0;
        glfwGetFramebufferSize(GetWindow(), &width, &height);
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(GetWindow(), &width, &height);
            glfwWaitEvents();
        }

        m_Device->WaitIdle();

        CreateSwapChain();
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
        m_InFlightFences[m_CurrentFrame].WaitForFence();

        uint32_t imageIndex;
        auto result = vkAcquireNextImageKHR(m_Device->GetVkDevice(), m_SwapChain->GetVkSwapChain(), UINT64_MAX, m_ImageAvailableSemaphore[m_CurrentFrame].GetVkSemaphore(), VK_NULL_HANDLE, &imageIndex);

        if (m_ImagesInFlight[imageIndex].GetVkFence() != VK_NULL_HANDLE)
            m_ImagesInFlight[imageIndex].WaitForFence();

        m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrame];

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("Failed to acquire swap chain image!");

        VkSubmitInfo submitInfo {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphore[] = { m_ImageAvailableSemaphore[m_CurrentFrame].GetVkSemaphore() };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphore;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_CommandBuffers->GetCommandBuffers()[imageIndex];

        VkSemaphore signalSemaphore[] = { m_RenderFinishedSemaphore[m_CurrentFrame].GetVkSemaphore() };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphore;

        m_InFlightFences[m_CurrentFrame].Reset();
        if (m_GraphicsQueue->Submit({ submitInfo }, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
            throw std::runtime_error("Failed to submit to graphics queue!");

        VkPresentInfoKHR presentInfoKhr {};
        presentInfoKhr.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfoKhr.waitSemaphoreCount = 1;
        presentInfoKhr.pWaitSemaphores = signalSemaphore;

        VkSwapchainKHR swapChain[] = { m_SwapChain->GetVkSwapChain() };
        presentInfoKhr.swapchainCount = 1;
        presentInfoKhr.pSwapchains = swapChain;
        presentInfoKhr.pImageIndices = &imageIndex;

        result = m_PresentQueue->PresentKHR(presentInfoKhr);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FrameBufferResized)
        {
            m_FrameBufferResized = false;
            RecreateSwapChain();
        }
        else if (result != VK_SUCCESS)
            throw std::runtime_error("Failed to present swap chain image!");

        m_PresentQueue->WaitIdle();

        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES;
    }

    void Application::OnWindowResized(int width, int height)
    {
        m_FrameBufferResized = true;
    }
}