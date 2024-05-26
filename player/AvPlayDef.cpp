#include "AvPlayDef.h"

void Global::ConvertToImage(AVFrame *pFrame)
{
    SwsContext *swsContext = sws_getContext(
           pFrame->width,
           pFrame->height,
           AV_PIX_FMT_YUV420P, // 源像素格式
                pFrame->width,
                pFrame->height,
                AV_PIX_FMT_RGB24,   // 目标像素格式
                SWS_BICUBIC,
                NULL,
                NULL,
                NULL
            );

    // 分配目标图像的内存
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pFrame->width, pFrame->height);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    AVPicture dstPic;
    avpicture_fill((AVPicture *)&dstPic, buffer, AV_PIX_FMT_RGB24, pFrame->width, pFrame->height);

    // 执行转换
    sws_scale(swsContext, (const uint8_t *const *)pFrame->data, pFrame->linesize, 0, pFrame->height, dstPic.data, dstPic.linesize);

    // 转换完成后释放SwsContext
    sws_freeContext(swsContext);

    QImage image((uchar*)dstPic.data[0], pFrame->width, pFrame->height, QImage::Format_RGB888);

    emit SigUpdateImage(image);

    av_frame_unref(pFrame);
}
