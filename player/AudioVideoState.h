#ifndef AUDIOVIDEOSTATE_H
#define AUDIOVIDEOSTATE_H

extern "C"
{
#include "libavformat/avformat.h"
}

#include <string>

struct AudioVideoState {
    AVFormatContext *pAvContext = NULL;   // 媒体文件上下文
};

#endif // AUDIOVIDEOSTATE_H
