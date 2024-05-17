#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <string>
#include "AudioVideoState.h"
#include "AvMsgQueue.h"

class MyPlayer
{
public:
    MyPlayer();
    void Prepare();
    void StreamOpen();

    AvMsgQueue m_queue;
    AudioVideoState m_avState;
};

#endif // MYPLAYER_H
