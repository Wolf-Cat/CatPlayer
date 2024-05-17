#ifndef AUDIOVIDEOSTATE_H
#define AUDIOVIDEOSTATE_H

#include "libavformat/avformat.h"
#include <string>

struct AudioVideoState {
    AVFormatContext *pAvContext = NULL;   // 媒体文件上下文
    std::string filePath;
};

#endif // AUDIOVIDEOSTATE_H
