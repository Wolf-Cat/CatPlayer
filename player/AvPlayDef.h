#ifndef AVPLAYDEF_H
#define AVPLAYDEF_H

#include "SDL.h"

struct FrameQueue {

};

struct AvPacketQueue {
    int packetCount;
    SDL_mutex *mutex;
    SDL_cond *cond;
};

inline char* av_myerr2str(int errnum)
{
    char tmp[AV_ERROR_MAX_STRING_SIZE] = {0};
    return av_make_error_string(tmp, AV_ERROR_MAX_STRING_SIZE, errnum);
}

#endif // AVPLAYDEF_H
