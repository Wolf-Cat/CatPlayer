#ifndef AVPLAYDEF_H
#define AVPLAYDEF_H

#include <QObject>
#include <QImage>

extern "C"
{
    #include "SDL.h"
    #include "SDL_mutex.h"
    #include "libavformat/avformat.h"
    #include "libavutil/imgutils.h"
    #include "libavutil/pixfmt.h"
    #include "libswscale/swscale.h"
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

signals:
    void SigUpdateImage(QImage img);
};

struct FrameQueue {

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
        pNewNode->pkt = pkt;
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
                *pOut = *(pFirstNode->pkt);
                count--;
                //av_packet_move_ref(pOut, pFirstNode->pkt);
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
