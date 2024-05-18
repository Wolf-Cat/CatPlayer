#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <string>
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
    void StreamOpen(const std::string& filePath);

    AvMsgQueue m_queue;
    AudioVideoState m_avState;
    std::string m_filePath;
};

#endif // MYPLAYER_H
