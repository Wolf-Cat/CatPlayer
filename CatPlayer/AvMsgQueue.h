#ifndef AV_MESSAGE_H
#define AV_MESSAGE_H

#include <mutex>
#include <condition_variable>

extern "C"
{

#include "libavutil/avutil.h"
#include "SDL.h"

}

enum AvMsgType {
    ERROR = -1,
    FLUSH = 0,     // 清空数据

    PREPARED = 1000,     // 播放器所需要资源已经准备，比如解复用器，解码器，渲染初始化
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

    std::mutex m_mutex;
    std::condition_variable m_condVar;
};

#endif // AV_MESSAGE_H
