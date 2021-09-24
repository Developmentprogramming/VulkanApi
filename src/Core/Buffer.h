//
// Created by aeternum on 9/23/21.
//

#ifndef VULKANAPI_BUFFER_H
#define VULKANAPI_BUFFER_H

#include <glm/glm.hpp>
#include <array>

#include "Device.h"
#include "CommandBuffers.h"

namespace VulkanApi
{
    class Buffer
    {
    public:
        virtual ~Buffer();

        virtual void Bind(uint32_t index) const = 0;

        inline Ref<Device> GetDevice() { return m_Device; }

        inline const VkBuffer& GetVkBuffer() const { return m_Buffer; }
        inline const VkDeviceMemory& GetVkDeviceMemory() const { return m_BufferMemory; }

    protected:
        Buffer(const Ref<Device>& device, const Ref<CommandBuffers>& commandBuffers, const Ref<Queue>& graphicsQueue, VkDeviceSize bufferSize, VkBufferUsageFlags usage, void* data);
        void CreateBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size);

    protected:
        Ref<Device> m_Device;
        Ref<CommandBuffers> m_CommandBuffers;
        Ref<Queue> m_GraphicsQueue;

    private:
        VkBuffer m_Buffer;
        VkDeviceMemory m_BufferMemory;
    };

    class VertexBuffer : public Buffer
    {
    public:
        VertexBuffer(const Ref<Device>& device, const Ref<CommandBuffers>& commandBuffers, const Ref<Queue>& graphicsQueue, void* data, uint64_t bufferSize);

        void Bind(uint32_t index) const override;
    };
}

#endif //VULKANAPI_BUFFER_H
