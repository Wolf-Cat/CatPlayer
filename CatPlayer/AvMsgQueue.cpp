#include "AvMsgQueue.h"

void AvMsgQueue::InitQueue()
{
    m_pMutex = SDL_CreateMutex();
    m_pCond = SDL_CreateCond();
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
    SDL_LockMutex(m_pMutex);
    m_abort = true;

    SDL_CondSignal(m_pCond);

    SDL_UnlockMutex(m_pMutex);
}

void AvMsgQueue::PutMessage(AvMsg *pMsg)
{
    if (nullptr == pMsg) {
        return;
    }

    SDL_LockMutex(m_pMutex);

    if (m_abort) {
        SDL_UnlockMutex(m_pMutex);
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

    SDL_CondSignal(m_pCond);

    SDL_UnlockMutex(m_pMutex);
}

AvMsg* AvMsgQueue::GetMessage(bool block)
{
    SDL_LockMutex(m_pMutex);

    AvMsg *pRetMsg = nullptr;
    for(;;) {
        if (m_abort) {
            SDL_UnlockMutex(m_pMutex);
            return pRetMsg;
        }

        if (m_pFirstMsg != nullptr) {
            pRetMsg = m_pFirstMsg;
            m_pFirstMsg = m_pFirstMsg->pNext;
            m_msgCount--;

            SDL_UnlockMutex(m_pMutex);
            return pRetMsg;

        }

        if (!block) {   // 非阻塞方式取数据
            break;
        } else {
            SDL_CondWait(m_pCond, m_pMutex);
        }
    }

    SDL_UnlockMutex(m_pMutex);
    return pRetMsg;
}

void AvMsgQueue::RemoveMessageByType(AvMsgType type)
{

}
