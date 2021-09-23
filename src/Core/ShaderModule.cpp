//
// Created by aeternum on 9/6/21.
//

#include "ShaderModule.h"

#include <stdexcept>
#include <cstring>
#include <fstream>

namespace VulkanApi
{

    ShaderModule::ShaderModule(const Ref<Device>& device, VkShaderStageFlagBits type, std::string code)
        : m_Device(device), m_Type(type), m_Code(std::move(code))
    {
        VkShaderModuleCreateInfo shaderCreateInfo {};
        shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize = (uint32_t)m_Code.size();
        shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(m_Code.data());

        if (vkCreateShaderModule(m_Device->GetVkDevice(), &shaderCreateInfo, nullptr, &m_ShaderModule) != VK_SUCCESS)
            throw std::runtime_error("Failed to create shader module!");
    }

    ShaderModule::~ShaderModule()
    {
        if (!m_Destroyed)
            Destroy();
    }

    void ShaderModule::Destroy()
    {
        vkDestroyShaderModule(m_Device->GetVkDevice(), m_ShaderModule, nullptr);
        m_Destroyed = true;
    }

    VkPipelineShaderStageCreateInfo ShaderModule::GetShaderStageCreateInfo() const
    {
        VkPipelineShaderStageCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo.stage = m_Type;
        createInfo.module = m_ShaderModule;
        createInfo.pName = "main";
        createInfo.pNext = nullptr;

        return createInfo;
    }

    // =====================================================================================
    // Shader ==============================================================================
    // =====================================================================================

    Shader::Shader(const Ref<Device>& device, const std::string &vertPath, const std::string &fragPath)
        : m_Device(device)
    {
        m_ShaderModules.reserve(2);

        auto vertCode = ReadFile(vertPath);
        auto fragCode = ReadFile(fragPath);

        m_ShaderModules.emplace_back(m_Device, VK_SHADER_STAGE_VERTEX_BIT, vertCode);
        m_ShaderModules.emplace_back(m_Device, VK_SHADER_STAGE_FRAGMENT_BIT, fragCode);
    }

    std::vector<VkPipelineShaderStageCreateInfo> Shader::GetVkShaderModules()
    {
        std::vector<VkPipelineShaderStageCreateInfo> ret;

        for (const auto& module : m_ShaderModules)
            ret.push_back(module.GetShaderStageCreateInfo());

        return std::move(ret);
    }

    std::string Shader::ReadFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
            throw std::runtime_error("Failed to open file!");

        size_t fileSize = (size_t)file.tellg();
        std::string fileContent;
        fileContent.resize(fileSize);
        file.seekg(0);
        file.read(fileContent.data(), (std::streamsize)fileSize);
        file.close();

        return fileContent;
    }
}
