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

namespace VulkanApi
{
    class Application : public Window
    {
    public:
        Application();
        virtual ~Application() = default;

        void Run();
        void DrawFrame();

    private:
        Ref<Instance> m_Instance;
        Ref<Surface> m_Surface;
        Ref<PhysicalDevice> m_PhysicalDevice;
        Ref<Device> m_Device;
        Ref<Queue> m_GraphicsQueue;
        Ref<Queue> m_PresentQueue;
        Ref<SwapChain> m_SwapChain;
        Ref<RenderPass> m_RenderPass;
        Ref<Shader> m_Shader;
        Ref<Pipeline> m_GraphicsPipeline;
        Ref<CommandPool> m_CommandPool;
        Ref<CommandBuffers> m_CommandBuffers;
        Ref<Semaphore> m_ImageAvailableSemaphore;
        Ref<Semaphore> m_RenderFinishedSemaphore;
    };
}

#endif //VULKANAPI_APPLICATION_H
