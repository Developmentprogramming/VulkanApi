//
// Created by aeternum on 9/9/21.
//

#ifndef VULKANAPI_APPLICATION_H
#define VULKANAPI_APPLICATION_H

#include "Core/Core.h"
#include "Core/Window.h"
#include "Core/Instance.h"
#include "Core/Surface.h"
#include "Core/Device.h"
#include "Core/SwapChain.h"
#include "Core/GraphicsPipline.h"
#include "Core/CommandPool.h"
#include "Core/CommandBuffers.h"
#include "Core/Semaphore.h"
#include "Core/Fence.h"
#include "Core/Buffer.h"

namespace VulkanApi
{
    class Application : public Window
    {
    public:
        Application();
        virtual ~Application() = default;

        void CreateSwapChain();
        void RecreateSwapChain();

        void Run();
        void DrawFrame();

        void OnWindowResized(int width, int height) override;

    private:
        Ref<Instance> m_Instance;
        Ref<Surface> m_Surface;
        Ref<PhysicalDevice> m_PhysicalDevice;
        Ref<Device> m_Device;
        Ref<Queue> m_GraphicsQueue, m_PresentQueue;
        Ref<SwapChain> m_SwapChain;
        Ref<RenderPass> m_RenderPass;
        Ref<Shader> m_Shader;
        Ref<Pipeline> m_GraphicsPipeline;
        Ref<CommandPool> m_CommandPool;
        Ref<CommandBuffers> m_CommandBuffers;
        std::vector<Semaphore> m_ImageAvailableSemaphore, m_RenderFinishedSemaphore;
        std::vector<Fence> m_InFlightFences, m_ImagesInFlight;

        Ref<VertexBuffer> m_VertexBuffer;

        bool m_FrameBufferResized = false, m_BuffersInitialized = false;
        uint32_t m_CurrentFrame = 0;
    };
}

#endif //VULKANAPI_APPLICATION_H
