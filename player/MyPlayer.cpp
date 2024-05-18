#include "MyPlayer.h"
#include "libavutil/log.h"


MyPlayer::MyPlayer()
{

}

/*stream_open的主要工作内容
 * 初始化SDL以允许音频输出
 * 初始化Frame队列
 * 初始化包Packet队列
 * 初始化时钟Clock
 * 创建解复用读取线程
 * 创建视频刷新线程
 * 初始化音量
*/

void MyPlayer::StreamOpen(const std::string& filePath)
{
    m_filePath = filePath;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0)
    {
        av_log(NULL, AV_LOG_DEBUG, "SDL init video audio timer success, %s", SDL_GetError());
    }
}
