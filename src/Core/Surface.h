//
// Created by aeternum on 9/5/21.
//

#ifndef VULKANAPI_SURFACE_H
#define VULKANAPI_SURFACE_H

#include "Instance.h"
#include "Window.h"
#include "Core.h"

namespace VulkanApi
{
    class Surface
    {
    public:
        Surface(Window& window, const Ref<Instance>& instance);
        virtual ~Surface();

        inline Window& GetWindow() { return m_Window; }
        inline Ref<Instance> GetInstance() { return m_Instance; }

        inline VkSurfaceKHR& GetVkSurface() { return m_Surface; }

    private:
        Window& m_Window;
        Ref<Instance> m_Instance;

        VkSurfaceKHR m_Surface;
    };
}


#endif //VULKANAPI_SURFACE_H
