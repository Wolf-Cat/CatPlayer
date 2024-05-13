#ifndef AV_MESSAGE_H
#define AV_MESSAGE_H

#include "SDL.h"
#include "libavutil/avutil.h"

enum AvMsgType {
    ERROR = -1,
    FLUSH = 0,     // 清空数据

    START_PLAY = 1001,   // 开始播放
    PAUSE_PLAY = 1002,   // 停止播放
    SEEK_PLAY = 1003     // 拖拽播放
};

struct AvMsg {
    int msgId;   // 指令：播放，暂停等
    int arg1 = -1;
    int arg2 = -1;
    void *pAvObj = nullptr;  // 用于释放Frame或者Packet等包
    struct AvMsg *pNext = nullptr;

    void FreeAvObj()
    {
        av_free(pAvObj);
    }
};

class AvMsgQueue {
   AvMsg *pFirstMsg = nullptr;
   AvMsg *pLastMsg = nullptr;
   int msgCount = 0;
   bool abort = false;    // 中止消息进入

   SDL_mutex *pMutex = nullptr;
   SDL_cond *pCond = nullptr;

   void InitQueue();
   void FlushQueue();
   void DestoryQueue();
   void AbortGetMessage();
   void PutMessage(AvMsg *pMsg);
   void GetMessage(bool block);
   void RemoveMessage(AvMsgType type);
}

#endif // AV_MESSAGE_H
