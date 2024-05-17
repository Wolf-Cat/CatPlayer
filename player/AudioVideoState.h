#ifndef AUDIOVIDEOSTATE_H
#define AUDIOVIDEOSTATE_H

#include "libavformat/avformat.h"

struct AudioVideoState {
    AVFormatContext *pAvContext = NULL;   // 媒体文件上下文
}

#endif // AUDIOVIDEOSTATE_H
