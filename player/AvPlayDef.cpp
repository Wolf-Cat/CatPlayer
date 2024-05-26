#include "AvPlayDef.h"

void Global::ConvertToImage(AVFrame *pFrame)
{
    /*
    SwsContext *sws_ctx = sws_getContext(
            pFrame->width, pFrame->height, AV_PIX_FMT_YUV420P,
            pFrame->width, pFrame->height, AV_PIX_FMT_RGB24,
            SWS_BICUBIC, NULL, NULL, NULL
        );

        // 分配RGB图像的缓冲区
        int rgb_width = av_image_get_linesize(AV_PIX_FMT_RGB24, pFrame->width, 0);
        uint8_t *rgb_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_RGB32, pFrame->width, pFrame->height));

        // 将YUV转换为RGB
        AVPicture rgb_picture;
        avpicture_fill((AVPicture *)&rgb_picture, rgb_buffer, AV_PIX_FMT_RGB24, pFrame->width, pFrame->height);
        sws_scale(sws_ctx, (const uint8_t *const *)pFrame->data, pFrame->linesize, 0, pFrame->height, rgb_picture.data, rgb_picture.linesize);

        // 将RGB数据转换为QImage
        QImage image((uchar*)rgb_picture.data[0], pFrame->width, pFrame->height, QImage::Format_RGB32);

        // 显示图像
        emit SigUpdateImage(image);

        // 释放资源
        av_free(rgb_buffer);
        sws_freeContext(sws_ctx);

    */

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

    if (swsContext == NULL) {
        return;
    }

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
