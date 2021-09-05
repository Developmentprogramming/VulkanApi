//
// Created by aeternum on 9/5/21.
//

#ifndef VULKANAPI_WINDOW_H
#define VULKANAPI_WINDOW_H

#include <cstdint>
#include <string>
struct GLFWwindow;

namespace VulkanApi
{
    class Window
    {
    public:
        Window(uint32_t width, uint32_t height, const std::string& title = "VulkanApi");
        virtual ~Window();

        inline uint32_t GetWidth() const { return m_Width; }
        inline uint32_t GetHeight() const { return m_Height; }
        inline std::string& GetTitle() { return m_Title; }
        inline const std::string& GetTitle() const { return m_Title; }

    private:
        GLFWwindow* m_Window;
        uint32_t m_Width, m_Height;
        std::string m_Title;

        static bool s_Initialized;
    };
}

#endif //VULKANAPI_WINDOW_H
