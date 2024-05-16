#ifndef AV_MESSAGE_H
#define AV_MESSAGE_H

extern "C"
{

#include "SDL.h"
#include "SDL_mutex.h"
#include "libavutil/avutil.h"

}

enum AvMsgType {
    ERROR = -1,
    FLUSH = 0,     // 清空数据

    START_PLAY = 1001,   // 开始播放
    PAUSE_PLAY = 1002,   // 停止播放
    SEEK_PLAY = 1003     // 拖拽播放
};

struct AvMsg {
    AvMsgType type = AvMsgType::ERROR;   // 指令：播放，暂停等
    int arg1 = -1;
    int arg2 = -1;
    void *pAvObj = nullptr;  // 用于释放Frame或者Packet等包
    struct AvMsg *pNext = nullptr;

    void FreeAvObj()
    {
        av_free(pAvObj);
        pAvObj = nullptr;
    }
};

class AvMsgQueue
{

public:
    void InitQueue();
    void FlushQueue();
    void DestoryQueue();
    void AbortGetMessage();
    void PutMessage(AvMsg *pMsg);
    AvMsg* GetMessage(bool block);
    void RemoveMessageByType(AvMsgType type);

private:
    AvMsg *m_pFirstMsg = nullptr;
    AvMsg *m_pLastMsg = nullptr;
    int m_msgCount = 0;
    bool m_abort = true;    // 中止消息进入

    SDL_mutex *m_pMutex = nullptr;
    SDL_cond *m_pCond = nullptr;
};

#endif // AV_MESSAGE_H
