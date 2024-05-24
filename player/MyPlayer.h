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
#include "libswresample/swresample.h"
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
    static int DecodeVideoThread(void *arg);
    void InitAVPacketQueue();
    void InitAVFrameQueue();
    void StreamComponentOpen(int streamIndex);   // 根据流索引得到流AVStream和打开相关组件
    void OpenAudioDevice();
    static void SdlAudioCallBack(void *userdata, uint8_t *stream, int needLen);
    int DecodeAudioFrame();

    AvMsgQueue m_queue;

public:
    std::shared_ptr<std::thread> m_pReadDataThread = nullptr;
    AVFormatContext *m_pFormatCtx = NULL;

    // 音频相关
    int m_audioIndex = -1;
    AVStream *m_audioStream = NULL;
    AVCodecContext *m_audioCodecCtx = NULL;
    AVCodec *m_audioCodec = NULL;
    AvPacketQueue m_audioPacketQueue;

    AVPacket m_audioPacket;
    AVFrame m_audioFrame;
    uint8_t *m_pAudioBuffer = NULL;
    uint8_t m_audioBufferSize = 0;      // 音频缓冲区大小字节数, 此处其实不用太纠结索引从0还是1开始，因为一个字节不会影响音频播放
    uint8_t m_audioBufferUsedSize = 0;  // 缓冲区已经使用的音频字节数

    struct SwrContext* m_audioSwrCtx = NULL;   // 音频重采样上下文

    // 视频相关
    int m_videoIndex = -1;
    AVStream *m_videoStream = NULL;
    AVCodecContext *m_videoCodecCtx = NULL;
    AVCodec *m_videoCodec = NULL;
    AvPacketQueue m_videoPacketQueue;
    int m_videoWidth = 0;
    int m_videoHeight = 0;

    std::shared_ptr<std::thread> m_pDecodeVideoThread = nullptr;

    // 音视频同步相关

    // 其余
    bool m_bQuit = false;
};

#endif // MYPLAYER_H
