//
// Created by aeternum on 9/6/21.
//

#ifndef VULKANAPI_SHADERMODULE_H
#define VULKANAPI_SHADERMODULE_H

#include "Device.h"

namespace VulkanApi
{
    enum class ShaderType
    {
        Vertex = VK_SHADER_STAGE_VERTEX_BIT,
        Fragment = VK_SHADER_STAGE_FRAGMENT_BIT
    };

    class ShaderModule
    {
    public:
        ShaderModule(Device& device, ShaderType type, const char* code);

        VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo(const std::string& entryName = "main") const;

        inline VkShaderModule& GetVkShaderModule() { return m_ShaderModule; }

    private:
        Device& m_Device;
        ShaderType m_Type;

        VkShaderModule m_ShaderModule;
    };

    class Shader
    {
    public:
        Shader(Device& device, const std::string& vertSrc, const std::string& fragSrc);

        std::vector<VkPipelineShaderStageCreateInfo> GetVkShaderModules();

    private:
        static std::string ReadFile(const std::string& filename);

    private:
        std::vector<ShaderModule> m_ShaderModules;
        Device& m_Device;
    };
}

#endif //VULKANAPI_SHADERMODULE_H
