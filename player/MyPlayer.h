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

    void InitAVPacketQueue();
    void InitAVFrameQueue();

    AvMsgQueue m_queue;
    std::string m_filePath;

private:
    std::shared_ptr<std::thread> m_pReadDataThread;
    AVFormatContext *m_pFormatCtx = NULL;
};

#endif // MYPLAYER_H
