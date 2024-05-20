#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <string>
#include <thread>
#include "AvPlayDef.h"
#include "AvMsgQueue.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
#include "SDL.h"

#ifdef __cplusplus
}
#endif

class MyPlayer
{
public:
    MyPlayer();
    void Prepare();
    void InitAvEnviroment(const std::string& filePath);
    static int ReadDataThread(void *arg);

    void InitAVPacketQueue();
    void InitAVFrameQueue();
    void StreamComponentOpen(int streamIndex);   // 根据流索引得到流AVStream和打开相关组件

    AvMsgQueue m_queue;
    std::string m_filePath;

public:
    std::shared_ptr<std::thread> m_pReadDataThread;
    AVFormatContext *m_pFormatCtx = NULL;
    int m_audioIndex = -1;
    int m_videoIndex = -1;

    // 音频相关
    AVStream *m_audioStream = NULL;
    AVCodecContext *m_audioCodecCtx = NULL;
    AVCodec *m_audioCodec = NULL;
    AvPacketQueue m_audioPacketQueue;

    // 视频相关
    AVStream *m_videoStream = NULL;
    AVCodecContext *m_videoCodecCtx = NULL;
    AVCodec *m_videoCodec = NULL;
    AvPacketQueue m_videoPacketQueue;
    int m_videoWidth = 0;
    int m_videoHeight = 0;

    // 音视频同步相关

    // 其余
    bool m_bQuit = false;
};

#endif // MYPLAYER_H
