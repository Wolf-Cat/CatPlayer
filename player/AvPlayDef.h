#ifndef AVPLAYDEF_H
#define AVPLAYDEF_H

struct FrameQueue {

};

struct AvPacketQueue {

};

inline char* av_myerr2str(int errnum)
{
    char tmp[AV_ERROR_MAX_STRING_SIZE] = {0};
    return av_make_error_string(tmp, AV_ERROR_MAX_STRING_SIZE, errnum);
}

#endif // AVPLAYDEF_H
