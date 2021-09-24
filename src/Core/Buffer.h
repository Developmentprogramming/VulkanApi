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

        virtual void Bind(const VkCommandBuffer& commandBuffer) const = 0;

        inline Ref<Device> GetDevice() { return m_Device; }

        inline const VkBuffer& GetVkBuffer() const { return m_Buffer; }
        inline const VkDeviceMemory& GetVkDeviceMemory() const { return m_BufferMemory; }
        inline const VkBufferUsageFlags GetVkBufferUsage() const { return m_BufferUsage; }

    protected:
        Buffer(const Ref<Device>& device, const Ref<CommandPool>& commandPool, const Ref<Queue>& graphicsQueue, VkDeviceSize bufferSize, VkBufferUsageFlags usage, void* data);
        void CreateBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size);

    protected:
        Ref<Device> m_Device;
        Ref<CommandPool> m_CommandPool;
        Ref<Queue> m_GraphicsQueue;

    private:
        VkBuffer m_Buffer;
        VkDeviceMemory m_BufferMemory;
        VkBufferUsageFlags m_BufferUsage;
    };

    class VertexBuffer : public Buffer
    {
    public:
        VertexBuffer(const Ref<Device>& device, const Ref<CommandPool>& commandPool, const Ref<Queue>& graphicsQueue, void* data, uint64_t bufferSize);

        void Bind(const VkCommandBuffer& commandBuffer) const override;
    };

    class IndexBuffer : public Buffer
    {
    public:
        IndexBuffer(const Ref<Device>& device, const Ref<CommandPool>& commandPool, const Ref<Queue>& graphicsQueue, void* data, uint32_t count);

        void Bind(const VkCommandBuffer& commandBuffer) const override;

        inline uint32_t GetIndexCount() const { return m_IndexCount; }

    private:
        uint32_t m_IndexCount;
    };
}

#endif //VULKANAPI_BUFFER_H
