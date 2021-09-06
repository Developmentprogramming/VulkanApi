//
// Created by aeternum on 9/6/21.
//

#include "ShaderModule.h"

#include <stdexcept>
#include <cstring>
#include <fstream>

namespace VulkanApi
{

    ShaderModule::ShaderModule(Device &device, ShaderType type, const char* code)
        : m_Device(device), m_Type(type)
    {
        VkShaderModuleCreateInfo shaderCreateInfo {};
        shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize = (uint32_t)strlen(code);
        shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code);

        if (vkCreateShaderModule(m_Device.GetVkDevice(), &shaderCreateInfo, nullptr, &m_ShaderModule) != VK_SUCCESS)
            throw std::runtime_error("Failed to create shader module!");
    }

    VkPipelineShaderStageCreateInfo ShaderModule::GetShaderStageCreateInfo(const std::string& entryName) const
    {
        VkPipelineShaderStageCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo.stage = static_cast<VkShaderStageFlagBits>(m_Type);
        createInfo.module = m_ShaderModule;
        createInfo.pName = entryName.data();

        return createInfo;
    }

    // =====================================================================================
    // Shader ==============================================================================
    // =====================================================================================

    Shader::Shader(Device &device, const std::string &vertPath, const std::string &fragPath)
        : m_Device(device)
    {
        auto vertCode = ReadFile(vertPath);
        auto fragCode = ReadFile(fragPath);

        m_ShaderModules.emplace_back(device, ShaderType::Vertex, vertCode.c_str());
        m_ShaderModules.emplace_back(device, ShaderType::Fragment, fragCode.c_str());
    }

    std::vector<VkPipelineShaderStageCreateInfo> Shader::GetVkShaderModules()
    {
        std::vector<VkPipelineShaderStageCreateInfo> ret(m_ShaderModules.size());

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
