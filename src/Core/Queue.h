//
// Created by aeternum on 9/6/21.
//

#ifndef VULKANAPI_QUEUE_H
#define VULKANAPI_QUEUE_H

#include "Device.h"
#include "Fence.h"

namespace VulkanApi
{
    class Queue
    {
    public:
        Queue() = default;
        inline void GetDeviceQueue(const Ref<Device>& device, uint32_t queueFamilyIndex, uint32_t queueIndex = 0) { vkGetDeviceQueue(device->GetVkDevice(), queueFamilyIndex, queueIndex, &m_Queue); };
        inline void WaitIdle() const { vkQueueWaitIdle(m_Queue); }
        inline VkResult Submit(const std::vector<VkSubmitInfo>& submitInfo, Fence& fence) { return vkQueueSubmit(m_Queue, submitInfo.size(), submitInfo.data(), fence.GetVkFence()); }
        inline VkResult Submit(const std::vector<VkSubmitInfo>& submitInfo) { return vkQueueSubmit(m_Queue, submitInfo.size(), submitInfo.data(), VK_NULL_HANDLE); }
        inline VkResult PresentKHR(const VkPresentInfoKHR& presentInfoKhr) { return vkQueuePresentKHR(m_Queue, &presentInfoKhr); }

        inline VkQueue& GetVkQueue() { return m_Queue; }

    private:
        VkQueue m_Queue;
    };
}

#endif //VULKANAPI_QUEUE_H
