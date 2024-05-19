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

    // 初始化音视频包队列（AVPacket)

    // 初始化音视频帧队列 （AVFrame）

    // 创建解复用器读取数据线程
    // m_pReadDataThread = std::make_shared<std::thread>(&MyPlayer::ReadDataThread, this);
}

void MyPlayer::ReadDataThread(void *arg)
{

}
