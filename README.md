# CatPlayer
一只猫的播放器

* 主要线程
1. 数据读取线程
2. 视频解码线程
3. 音频解码线程
4. 音频播放线程

* 主要队列
1. 界面层和播放器之间的消息队列 （起到桥接作用）
2. 视频AVPacket队列
3. 视频Frame队列
4. 音频AVPacket队列
5. 音频Frame队列

* 音视频同步
以音频为基准，视频向音频同步

* 使用到的FFmpeg重要的函数
avformat_alloc_context
avformat_open_input      // 打开文件，如果是网络文件则创建网络链接等
avformat_find_stream_info
stream_component_open
av_read_frame
avcodec_send_packet
avcodec_receive_frame

* 使用到的FFmpeg重要结构体
AVFormatContext
AVInputFormat
AVOutputFormat 

AVStream
AVPacket
AVFrame

AVCodecContext
AVCodec
AVCodecParameters

* 使用的SDL重要函数
SDL_OpenAudio

* PCM采样格式
AV_SAMPLE_FMT_S16

* 一些时间获取函数
int64_t av_gettime()           //从公元1970年1月1日0时0分0秒开始的微秒值
int64_t av_gettime_relative()  // 返回开机后开始的微妙值



