#ifndef AVPLAYDEF_H
#define AVPLAYDEF_H

#include <QObject>
#include <QImage>
#include <QLabel>
#include <queue>

extern "C"
{
    #include "SDL.h"
    #include "SDL_mutex.h"
    #include "libavformat/avformat.h"
    #include "libavutil/imgutils.h"
    #include "libavutil/pixfmt.h"
    #include "libswscale/swscale.h"
}

namespace {
    const int VIDEO_FRAME_QUEUE_SIZE = 9;  // 最多缓存9帧
}

class Global : public QObject {

    Q_OBJECT
public:
    static Global& GetInstance()
    {
        static Global instance;
        return instance;
    }

    void ConvertToImage(AVFrame *pFrame);

    QLabel *label;
signals:
    void SigUpdateImage(QImage img);
};

struct VideoFrame {   // 解码后的AVFrame
    AVFrame *frame = NULL;
    double curClock = 0;   // 该帧目前的时钟
    double duration = 0;   // 该帧视频播放时长，单位秒
};

struct DecodeVideoFrameQueue {
    std::queue<VideoFrame> picQue;
    int count = 0;
    bool isAbort = false;
    SDL_mutex *mutex = NULL;
    SDL_cond *cond = NULL;

    void PushFramePic(AVFrame *pFrame, double curClock, double duration) {
        if (pFrame == NULL) {
            return;
        }

        VideoFrame vFrame;
        vFrame.frame = av_frame_alloc();
        av_frame_move_ref(vFrame.frame, pFrame);
        vFrame.curClock = curClock;
        vFrame.duration = duration;

        SDL_LockMutex(mutex);
        picQue.emplace(vFrame);

        SDL_CondSignal(cond);
        SDL_UnlockMutex(mutex);
    }

    bool GetFramePic(VideoFrame &vOutFrame) {
        SDL_LockMutex(mutex);

        for (;;) {
            if (picQue.empty()) {
                SDL_CondWait(cond, mutex);
            }

            av_frame_move_ref(vOutFrame.frame, picQue.front().frame);
            vOutFrame.curClock = picQue.front().curClock;
            vOutFrame.duration = picQue.front().duration;
            picQue.pop();
            break;
        }

        SDL_UnlockMutex(mutex);

        return true;
    }
};

struct AVPacketNode {
    AVPacket *pkt = NULL;
    AVPacketNode *next = NULL;
};

struct AvPacketQueue {
    int count = 0;
    AVPacketNode *pFirstNode = NULL;
    AVPacketNode *pLastNode = NULL;
    SDL_mutex *mutex = NULL;
    SDL_cond *cond = NULL;

    void PutPacket(AVPacket *pkt)
    {
        if (pkt == NULL) {
            return;
        }

        SDL_LockMutex(mutex);
        AVPacketNode *pNewNode = (AVPacketNode *)malloc(sizeof(AVPacketNode));
        pNewNode->pkt = av_packet_alloc();
        av_packet_move_ref(pNewNode->pkt, pkt);
        pNewNode->next = NULL;
        if (pLastNode == NULL) {
            pFirstNode = pNewNode;
        } else {
            pLastNode->next = pNewNode;
        }

        pLastNode = pNewNode;
        count++;

        SDL_CondSignal(cond);
        SDL_UnlockMutex(mutex);
    }

    int GetPacket(bool isblock, AVPacket *pOut)
    {
        int ret = -100;
        SDL_LockMutex(mutex);

        for(;;)
        {
            if (pFirstNode != NULL) {
                av_packet_move_ref(pOut, pFirstNode->pkt);
                count--;
                pFirstNode = pFirstNode->next;
                //free(pFirstNode);
                ret = 0;
                break;
            }

            if (!isblock) {
                ret = -1;
                break;
            } else {
                SDL_CondWait(cond, mutex);
            }
        }

        SDL_UnlockMutex(mutex);

        return ret;
    }
};

inline char* av_myerr2str(int errnum)
{
    char tmp[AV_ERROR_MAX_STRING_SIZE] = {0};
    return av_make_error_string(tmp, AV_ERROR_MAX_STRING_SIZE, errnum);
}

#endif // AVPLAYDEF_H
