#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <string>
#include <thread>
#include "AudioVideoState.h"
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
    void ReadDataThread(void *arg);

    void InitAVPacketQueue(AVMediaType codec_type);
    void InitAVFrameQueue();
    void StreamComponentOpen(int streamIndex);   // 根据流索引得到流AVStream和打开相关组件

    AvMsgQueue m_queue;
    std::string m_filePath;

private:
    std::shared_ptr<std::thread> m_pReadDataThread;
    AVFormatContext *m_pFormatCtx = NULL;
    int m_audioIndex = -1;
    int m_videoIndex = -1;

    // 音频相关
    AVStream *m_audioStream = NULL;
    AVCodecContext *m_audioCodecCtx = NULL;
    AVCodec *m_audioCodec = NULL;

    // 视频相关
    AVStream *m_videoStream = NULL;
    AVCodecContext *m_videoCodecCtx = NULL;
    AVCodec *m_videoCodec = NULL;

    // 音视频同步相关
};

#endif // MYPLAYER_H
