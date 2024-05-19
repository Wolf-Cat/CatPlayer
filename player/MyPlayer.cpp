#include "MyPlayer.h"
#include "libavutil/log.h"
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
    if (avformat_open_input(&m_pFormatCtx, filePath.c_str(), NULL, NULL) < 0)
    {
        qDebug() << filePath.c_str() <<" avformat_open_input sucess ";
    }

    // 初始化音视频包队列（AVPacket)

    // 初始化音视频帧队列 （AVFrame）

    // 创建解复用器读取数据线程
    // m_pReadDataThread = std::make_shared<std::thread>(&MyPlayer::ReadDataThread, this);
}

void MyPlayer::ReadDataThread(void *arg)
{

}
