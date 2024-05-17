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

private:
    std::string m_path= "testVideo.mp4";
    AvMsgQueue m_queue;
};

#endif // MYPLAYER_H
