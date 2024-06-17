#include "MyPlayer.h"
#include "libavutil/log.h"
#include "libavutil/avutil.h"
#include <QDebug>

namespace
{
    int SDL_AUDIO_BUFFER_DEFAULT_SIZE = 1024;
}

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
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0)
    {
        av_log(NULL, AV_LOG_DEBUG, "SDL init video audio timer success, %s", SDL_GetError());
    }

    // 获得媒体文件上下文
    int ret = 100;
    ret = avformat_open_input(&m_pFormatCtx, "testVideo2.mp4", NULL, NULL);
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
        if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && m_audioIndex == -1)
        {
            m_audioIndex = i;
            StreamComponentOpen(m_audioIndex);
        }

        if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO && m_videoIndex == -1)
        {
            m_videoIndex = i;
            StreamComponentOpen(m_videoIndex);
        }

        if (m_audioIndex != -1 && m_videoIndex != -1) {
            break;
        }
    }

    av_dump_format(m_pFormatCtx, 0, "movie.mp4", 0);

    m_audioPacket = av_packet_alloc();
    m_audioFrame = av_frame_alloc();

    // 初始化音视频包队列（AVPacket)
    InitAVPacketQueue();

    // 初始化音视频帧队列 （AVFrame）

    // 创建解复用器读取数据线程
    m_pReadDataThread = std::make_shared<std::thread>(&MyPlayer::ReadDataThread, this);

    // 创建视频包解码线程
    m_pDecodeVideoThread = std::make_shared<std::thread>(&MyPlayer::DecodeVideoThread, this);

    m_pSyncVideoRender = std::make_shared<std::thread>(&MyPlayer::SyncVideoThread, this);

    // 加载SDL所需音频参数, 并打开扬声器
    OpenAudioDevice();
}

int MyPlayer::ReadDataThread(void *arg)
{
    MyPlayer *pPlayer = (MyPlayer *)arg;
    AVPacket *pkt = av_packet_alloc();
    for (;;) {
        if (pPlayer->m_bQuit) {
            break;
        }

        int ret = av_read_frame(pPlayer->m_pFormatCtx, pkt);
        if (ret == 0)
        {
            if (pkt->stream_index == pPlayer->m_videoIndex) {
                pPlayer->m_videoPacketQueue.PutPacket(pkt);
            } else if (pkt->stream_index == pPlayer->m_audioIndex) {
                pPlayer->m_audioPacketQueue.PutPacket(pkt);
            } else {
                av_packet_unref(pkt);
            }
        } else if (ret < 0) {
            if(pPlayer->m_pFormatCtx->pb->error == 0) {
                SDL_Delay(100);
            }
        }
    }

    return 0;
}

int MyPlayer::DecodeVideoThread(void *arg)
{
    MyPlayer *pPlayer = (MyPlayer *)arg;

    int ret = -1;
    AVPacket pkt;
    AVFrame *pFrame = av_frame_alloc();

    // AVStream->time_base 这是表示帧时间戳的基本时间单位（以秒为单位）。

    AVRational time_base = pPlayer->m_videoStream->time_base;   // 1 / 90000, AVStream中的time_base精度更准，以秒为单位
    AVRational fps = av_guess_frame_rate(pPlayer->m_pFormatCtx, pPlayer->m_videoStream, NULL);  // 帧率，60
    double perVideoFrameDuration = 0;   // 一帧的播放时长
    double frameConvertPts = 0;        // 最后转化的pts

    for (;;) {

        if (pPlayer->m_videoPacketQueue.GetPacket(false, &pkt) == 0) {
            ret = avcodec_send_packet(pPlayer->m_videoCodecCtx, &pkt);
            av_packet_unref(&pkt);

            if (ret < 0) {
                QString err = QString(av_myerr2str(ret));
                qDebug() << "avcodec_decode_video2 err: " << err;
                continue;
            }

            while (ret >= 0) {
                ret = avcodec_receive_frame(pPlayer->m_videoCodecCtx, pFrame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    av_frame_unref(pFrame);
                    return ret;
                }

                if (fps.den > 0 && fps.num > 0) {
                    perVideoFrameDuration = av_q2d({fps.den, fps.num});  // 将AVRational 对象由分数转换为小数，得到每一帧的持续时长，得到结果单位是秒;
                    // 一秒除以帧率，得到的每一帧播放持续时长
                } else {
                    perVideoFrameDuration = 0;
                }

                // 将源pts转化为以秒为单位的时间
                if (pFrame->pts == AV_NOPTS_VALUE) {
                    frameConvertPts = NAN;
                } else {
                    frameConvertPts = pFrame->pts * av_q2d(time_base);  //  (a.num / (double) a.den)
                }

                // 同步当前的video clock;
                //pPlayer->SynchronizeVideoClock(pFrame, frameConvertPts);
                pPlayer->m_decodeVFrameQue.PushFramePic(pFrame, frameConvertPts, perVideoFrameDuration);

                //Global::GetInstance().ConvertToImage(pFrame);
            }

            SDL_Delay(10);
        }
    }

}

int MyPlayer::SyncVideoThread(void *arg)
{
    MyPlayer *pPlayer = (MyPlayer *)arg;

    VideoFrame vFrame;
    vFrame.frame = av_frame_alloc();

    for (;;) {
        int idiff = 5;
        if (pPlayer->m_decodeVFrameQue.GetFramePic(vFrame))
        {
            //Global::GetInstance().ConvertToImage(vFrame.frame);
            //av_frame_move_ref(m_pVFrame, vFrame.frame);

            if (vFrame.curClock + vFrame.duration < pPlayer->m_audioClock) {
                Global::GetInstance().ConvertToImage(vFrame.frame);   // 音频的时钟比当前帧时间快，则迅速播放视频
            } else {   // 视频更快，则需要暂停与音频时钟的差值时间再去播放视频
                double diff = vFrame.curClock + vFrame.duration - pPlayer->m_audioClock;
                idiff = int(diff * 1000);
                SDL_Delay(idiff);
                Global::GetInstance().ConvertToImage(vFrame.frame);
            }
        }
    }
}

void MyPlayer::InitAVPacketQueue()
{
    m_audioPacketQueue.mutex = SDL_CreateMutex();
    m_audioPacketQueue.cond = SDL_CreateCond();

    m_videoPacketQueue.mutex = SDL_CreateMutex();
    m_videoPacketQueue.cond = SDL_CreateCond();

    m_decodeVFrameQue.mutex = SDL_CreateMutex();
    m_decodeVFrameQue.cond = SDL_CreateCond();
}

void MyPlayer::StreamComponentOpen(int streamIndex)
{
    if (m_pFormatCtx == nullptr || streamIndex < 0 || streamIndex >= m_pFormatCtx->nb_streams) {
        return;
    }

    AVCodecID codec_id = m_pFormatCtx->streams[streamIndex]->codecpar->codec_id;
    AVCodec *pCodec = avcodec_find_decoder(codec_id);             // 解码器
    int ret = 0;
    AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);   // 创建解码器上下文

    ret = avcodec_parameters_to_context(pCodecCtx, m_pFormatCtx->streams[streamIndex]->codecpar);
    if (ret < 0) {
        return;
    }

    if (pCodecCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
        m_audioStream = m_pFormatCtx->streams[streamIndex];
        m_audioCodecCtx = pCodecCtx;
        m_audioCodec = pCodec;

    } else if (pCodecCtx->codec_type == AVMEDIA_TYPE_VIDEO) {
        m_videoStream = m_pFormatCtx->streams[streamIndex];
        m_videoCodecCtx = pCodecCtx;
        m_videoCodec = pCodec;

        m_videoWidth = m_videoCodecCtx->width;
        m_videoHeight = m_videoCodecCtx->height;
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)  // Initialize the AVCodecContext to use the given AVCodec
    {
        qDebug() << "avcodec_open2 failed";
    }
}

void MyPlayer::OpenAudioDevice()
{
    SDL_AudioSpec wantedSpec, specOut;
    wantedSpec.freq = m_audioCodecCtx->sample_rate;
    wantedSpec.format = AUDIO_S16SYS;
    wantedSpec.channels = m_audioCodecCtx->channels;
    wantedSpec.silence = 0;
    wantedSpec.samples = SDL_AUDIO_BUFFER_DEFAULT_SIZE;
    wantedSpec.callback = SdlAudioCallBack;
    wantedSpec.userdata = (void *)this;

    int ret = SDL_OpenAudio(&wantedSpec, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "SDL_OpenAudio : %s\n", SDL_GetError());
    }

    // 开启音频
    SDL_PauseAudio(0);
}

void MyPlayer::SdlAudioCallBack(void *userdata, Uint8 *stream, int needLen /*扬声器需要的数据字节数*/)     // 会在SDL创建的线程中去回调运行
{
    MyPlayer *player = (MyPlayer *)userdata;
    int actualRetLen = 0;   //    真正给到扬声器所需要的数据的大小
    int decodecAudioSize = 0;   // 解码后的音频字节数

    while (needLen > 0) {
        if (player->m_audioBufferUsedSize >= player->m_audioBufferSize) {   // 缓冲区数据已经用完，需要解码新数据
            decodecAudioSize = player->DecodeAudioFrame();
            if (decodecAudioSize < 0) {   // 解码失败，补静音包
                player->m_audioBufferSize = SDL_AUDIO_BUFFER_DEFAULT_SIZE;
                player->m_pAudioBuffer = NULL;
            } else {
                player->m_audioBufferSize = decodecAudioSize;
            }

            player->m_audioBufferUsedSize = 0;
        }

        // 目前缓冲区剩余的数据
        int curRmainLen = player->m_audioBufferSize - player->m_audioBufferUsedSize;

        // 最多给到扬声器本此实际需要的数据量
        actualRetLen = (curRmainLen > needLen) ? needLen : curRmainLen;

        if (player->m_pAudioBuffer != NULL) {
            memcpy(stream, (uint8_t *)(player->m_pAudioBuffer + player->m_audioBufferUsedSize), actualRetLen);
        } else {
            memset(stream, 0, actualRetLen);
        }

        needLen -= actualRetLen;
        stream += actualRetLen;
        player->m_audioBufferUsedSize += actualRetLen;
    }
}

int MyPlayer::DecodeAudioFrame()
{
    int retDataSize = -1;   // 实际解码数据的大小
    int ret = -1;

    for (;;) {

        if (m_audioPacketQueue.GetPacket(false, m_audioPacket) < 0) {
            break;
        }

        ret = avcodec_send_packet(m_audioCodecCtx, m_audioPacket);
        av_packet_unref(m_audioPacket);

        if (ret < 0) {
            return ret;
        }

        while (ret >= 0) {
            ret = avcodec_receive_frame(m_audioCodecCtx, m_audioFrame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                break;
            } else if (ret < 0) {
                QString str = QString(av_myerr2str(ret));
                av_frame_unref(m_audioFrame);
                return ret;
            }

            if (m_audioSwrCtx == NULL) {
                if (m_audioCodecCtx->sample_fmt != AV_SAMPLE_FMT_S16) {
                    m_audioSwrCtx = swr_alloc();

                    swr_alloc_set_opts(m_audioSwrCtx, AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16,
                                       m_audioCodecCtx->sample_rate, m_audioCodecCtx->channel_layout,
                                       m_audioCodecCtx->sample_fmt, m_audioCodecCtx->sample_rate,
                                       0, NULL);

                    swr_init(m_audioSwrCtx);
                }
            }

            // 进行重采样
            const uint8_t **inData =(const uint8_t **)m_audioFrame->extended_data;
            uint8_t **outDatabuff = &m_pAudioBuffer;
            int outCount = m_audioFrame->nb_samples + 256;   // 输出的采样点个数, 256为冗余值，最多不超过 nb_samples + 256
            int outSize = av_samples_get_buffer_size(NULL, m_audioFrame->channels, outCount, AV_SAMPLE_FMT_S16, 0);

            av_fast_malloc(&m_pAudioBuffer, &m_audioBufferSize, outSize);

            // 重采样后的采样点个数
            int nCovertLen = swr_convert(m_audioSwrCtx, outDatabuff, outCount, inData,
                                         m_audioFrame->nb_samples);

            retDataSize = nCovertLen * m_audioFrame->channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

            m_audioClock = (m_audioFrame->pts) * av_q2d(m_audioStream->time_base);  // + (double) m_audioFrame->nb_samples / m_audioFrame->sample_rate;  // 1024 / 44100 + 23ms
            qDebug() <<"audioFrame pts: " << m_audioFrame->pts << "Get audioClock: " << m_audioClock << "s";

            av_frame_unref(m_audioFrame);

            return retDataSize;
        }
    }

    return 0;
}

void MyPlayer::SynchronizeVideoClock(AVFrame *srcFrame, double &pts)
{
    if (pts == 0) {
        pts = m_videoClock;
    } else {
        m_videoClock = pts;
    }

    /*
    double frameDelayTime = av_q2d(m_videoCodecCtx->time_base);   // den = 2997, num = 1
    frameDelayTime += srcFrame->repeat_pict * (frameDelayTime * 0.5);  // repeat_pict 该帧是否是重复帧，并且重复了几次
    m_videoClock += frameDelayTime;
    */
}
