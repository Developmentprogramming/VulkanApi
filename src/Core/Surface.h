//
// Created by aeternum on 9/5/21.
//

#ifndef VULKANAPI_SURFACE_H
#define VULKANAPI_SURFACE_H

#include "Instance.h"
#include "Window.h"

namespace VulkanApi
{
    class Surface
    {
        Surface();

        inline Window& GetWindow() { return m_Window; }
        inline Instance& GetInstance() { return m_Instance; }

    private:
        Window m_Window;
        Instance m_Instance;

        VkSurfaceKHR m_Surface;
    };
}


#endif //VULKANAPI_SURFACE_H
