#include "AvMsgQueue.h"

void AvMsgQueue::InitQueue()
{
    m_abort = true;  // 初始化时中止消息循环
}

void AvMsgQueue::FlushQueue()
{

}

void AvMsgQueue::DestoryQueue()
{

}

void AvMsgQueue::AbortGetMessage()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_abort = true;
    m_condVar.notify_one();
}

void AvMsgQueue::PutMessage(AvMsg *pMsg)
{
    if (nullptr == pMsg) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_abort) {
        return;
    }

    if (m_pFirstMsg == nullptr) {
        m_pFirstMsg = pMsg;
    } else {
        m_pLastMsg->pNext = pMsg;
    }

    m_pLastMsg = pMsg;
    m_pLastMsg->pNext = nullptr;
    m_msgCount++;

    m_condVar.notify_one();
}

AvMsg* AvMsgQueue::GetMessage(bool block)
{

    std::lock_guard<std::mutex> lock(m_mutex);

    AvMsg *pRetMsg = nullptr;
    for(;;) {
        if (m_abort) {
            return pRetMsg;
        }

        if (m_pFirstMsg != nullptr) {
            pRetMsg = m_pFirstMsg;
            m_pFirstMsg = m_pFirstMsg->pNext;
            m_msgCount--;

            return pRetMsg;

        }

        if (!block) {   // 非阻塞方式取数据
            break;
        } else {
            m_condVar.notify_one();
        }
    }

    return pRetMsg;
}

void AvMsgQueue::RemoveMessageByType(AvMsgType type)
{

}
