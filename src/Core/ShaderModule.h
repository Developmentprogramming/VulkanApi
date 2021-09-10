//
// Created by aeternum on 9/6/21.
//

#ifndef VULKANAPI_SHADERMODULE_H
#define VULKANAPI_SHADERMODULE_H

#include "Device.h"

namespace VulkanApi
{
    class ShaderModule
    {
    public:
        ShaderModule(Device& device, VkShaderStageFlagBits type, std::string code);
        virtual ~ShaderModule();

        VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo() const;

        inline VkShaderModule& GetVkShaderModule() { return m_ShaderModule; }

    private:
        Device& m_Device;
        VkShaderStageFlagBits m_Type;
        std::string m_Code;

        VkShaderModule m_ShaderModule;
    };

    class Shader
    {
    public:
        Shader(Device& device, const std::string& vertSrc, const std::string& fragSrc);

        std::vector<VkPipelineShaderStageCreateInfo> GetVkShaderModules();
        inline std::vector<ShaderModule>& GetShaderModules() { return m_ShaderModules; }

    private:
        static std::string ReadFile(const std::string& filename);

    private:
        std::vector<ShaderModule> m_ShaderModules;
        Device& m_Device;
    };
}

#endif //VULKANAPI_SHADERMODULE_H
