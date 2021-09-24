//
// Created by aeternum on 9/23/21.
//

#include "Buffer.h"

#include <stdexcept>
#include <cstring>
#include "Queue.h"

namespace VulkanApi
{

    Buffer::Buffer(const Ref<Device>& device, const Ref<CommandBuffers>& commandBuffers, const Ref<Queue>& graphicsQueue,
                   VkDeviceSize bufferSize, VkBufferUsageFlags usage, void* data)
        : m_Device(device), m_CommandBuffers(commandBuffers), m_GraphicsQueue(graphicsQueue)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingMemory;
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

        void* tempData;
        vkMapMemory(m_Device->GetVkDevice(), stagingMemory, 0, bufferSize, 0, &tempData);
        memcpy(tempData, data, (size_t)bufferSize);
        vkUnmapMemory(m_Device->GetVkDevice(), stagingMemory);

        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_BufferMemory);
        CopyBuffer(stagingBuffer, m_Buffer, bufferSize);

        vkDestroyBuffer(m_Device->GetVkDevice(), stagingBuffer, nullptr);
        vkFreeMemory(m_Device->GetVkDevice(), stagingMemory, nullptr);
    }

    Buffer::~Buffer()
    {
        vkDestroyBuffer(m_Device->GetVkDevice(), m_Buffer, nullptr);
        vkFreeMemory(m_Device->GetVkDevice(), m_BufferMemory, nullptr);
    }

    void Buffer::CreateBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                              VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferCreateInfo {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = bufferSize;
        bufferCreateInfo.usage = usage;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_Device->GetVkDevice(), &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS)
            throw std::runtime_error("Failed to create buffer!");

        VkMemoryRequirements memoryRequirements {};
        vkGetBufferMemoryRequirements(m_Device->GetVkDevice(), buffer, &memoryRequirements);

        VkMemoryAllocateInfo allocInfo {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(m_Device->GetVkDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate buffer memory!");

        vkBindBufferMemory(m_Device->GetVkDevice(), buffer, bufferMemory, 0);
    }

    uint32_t Buffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memoryProperties {};
        vkGetPhysicalDeviceMemoryProperties(m_Device->GetPhysicalDevice()->GetVkPhysicalDevice(), &memoryProperties);

        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
            if ((typeFilter & (1 << i)) && ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties))
                return i;

        throw std::runtime_error("Failed to find suitable memory type!");
    }

    void Buffer::CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size)
    {
        VkCommandBufferAllocateInfo commandBufferAllocateInfo {};
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandPool = m_CommandBuffers->GetCommandPool()->GetVkCommandPool();
        commandBufferAllocateInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(m_Device->GetVkDevice(), &commandBufferAllocateInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion {};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;

        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        m_GraphicsQueue->Submit({ submitInfo });
        m_GraphicsQueue->WaitIdle();

        vkFreeCommandBuffers(m_Device->GetVkDevice(), m_CommandBuffers->GetCommandPool()->GetVkCommandPool(), 1, &commandBuffer);
    }

    // ============================================================================================
    // Vertex Buffer ==============================================================================
    // ============================================================================================

    VertexBuffer::VertexBuffer(const Ref<Device> &device, const Ref<CommandBuffers>& commandBuffers, const Ref<Queue> &graphicsQueue, void *data, uint64_t bufferSize)
        : Buffer(device, commandBuffers, graphicsQueue, bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data)
    {
    }

    void VertexBuffer::Bind(uint32_t index) const
    {
        VkBuffer vertexBuffers[] = { GetVkBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(m_CommandBuffers->GetCommandBuffers()[index], 0, 1, vertexBuffers, offsets);
    }
}