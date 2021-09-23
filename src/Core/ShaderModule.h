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
        ShaderModule(const Ref<Device>& device, VkShaderStageFlagBits type, std::string code);
        virtual ~ShaderModule();
        void Destroy();

        VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo() const;

        inline VkShaderModule& GetVkShaderModule() { return m_ShaderModule; }

    private:
        Ref<Device> m_Device;
        VkShaderStageFlagBits m_Type;
        std::string m_Code;
        bool m_Destroyed = false;

        VkShaderModule m_ShaderModule;
    };

    class Shader
    {
    public:
        Shader(const Ref<Device>& device, const std::string& vertSrc, const std::string& fragSrc);

        std::vector<VkPipelineShaderStageCreateInfo> GetVkShaderModules();
        inline std::vector<ShaderModule>& GetShaderModules() { return m_ShaderModules; }
        inline void ReleaseModules() { m_ShaderModules.clear(); }

    private:
        static std::string ReadFile(const std::string& filename);

    private:
        Ref<Device> m_Device;

        std::vector<ShaderModule> m_ShaderModules;
    };
}

#endif //VULKANAPI_SHADERMODULE_H
