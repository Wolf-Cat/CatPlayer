#include "MyPlayer.h"
#include "libavutil/log.h"
#include "libavutil/avutil.h"
#include "AvPlayDef.h"
#include <QDebug>

MyPlayer::MyPlayer()
{

}

/* InitAvEnviroment的主要工作内容
 * 初始化SDL以允许音频输出
 * 初始化Frame队列
 * 初始化包Packet队列
 * 初始化时钟Clock
 * 创建解复用读取线程
 * 创建视频刷新线程
 * 初始化音量
*/

void MyPlayer::InitAvEnviroment(const std::string& filePath)
{
    m_filePath = filePath;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0)
    {
        av_log(NULL, AV_LOG_DEBUG, "SDL init video audio timer success, %s", SDL_GetError());
    }

    // 获得媒体文件上下文
    int ret = 100;
    ret = avformat_open_input(&m_pFormatCtx, "F:/CatPlayer/build-CatPlayer-Desktop_Qt_5_10_1_MinGW_32bit-Debug/debug/testVideo.mp4", NULL, NULL);
    if (ret < 0)
    {
       QString err = av_myerr2str(ret);
       qDebug() << "avformat_open_input failed: " << err;
    }

    // 从打开的多媒体文件中获取流相关的信息
    ret = avformat_find_stream_info(m_pFormatCtx, NULL);
    if (ret < 0)
    {
        QString err = av_myerr2str(ret);
        qDebug() << "avformat_find_stream_info failed: " << err;
    }

    for (int i = 0; i < m_pFormatCtx->nb_streams; i++) {
        if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && m_audioIndex)
        {
            m_audioIndex = i;
            StreamComponentOpen(m_audioIndex);
        }

        if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO && m_videoIndex)
        {
            m_videoIndex = i;
            StreamComponentOpen(m_videoIndex);
        }
    }

    av_dump_format(m_pFormatCtx, 0, "F:/CatPlayer/build-CatPlayer-Desktop_Qt_5_10_1_MinGW_32bit-Debug/debug/testVideo.mp4", 0);


    // 初始化音视频包队列（AVPacket)

    // 初始化音视频帧队列 （AVFrame）

    // 创建解复用器读取数据线程
    // m_pReadDataThread = std::make_shared<std::thread>(&MyPlayer::ReadDataThread, this);
}

void MyPlayer::ReadDataThread(void *arg)
{

}

void MyPlayer::StreamComponentOpen(int streamIndex)
{
    if (m_pFormatCtx == nullptr || streamIndex < 0 || streamIndex >= m_pFormatCtx->nb_streams) {
        return;
    }

    AVCodecContext *pCodecCtx = avcodec_alloc_context3(NULL);   // 创建解码器上下文

    int ret = 0;
    ret = avcodec_parameters_to_context(pCodecCtx, m_pFormatCtx->streams[streamIndex]->codecpar);
    if (ret < 0) {
        return;
    }

    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);             // 解码器
    if (pCodecCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
        m_audioStream = m_pFormatCtx->streams[streamIndex];
    } else if (pCodecCtx->codec_type == AVMEDIA_TYPE_VIDEO) {
        m_videoStream = m_pFormatCtx->streams[streamIndex];
    }
}
