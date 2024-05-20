# CatPlayer
一只猫的播放器

* 播放器架构组成
界面UI模块
桥接控制模块 : 用于界面层和播放器之间的消息传递，采用消息队列的方式
播放器模块MyPlayer : 读取音视频数据，解码，播放，音视频同步

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
avformat_open_input        // 打开文件，如果是网络文件则创建网络链接等
avformat_find_stream_info  // 检索流信息
avcodec_open2              // 打开编解码器
avcodec_send_packet        // 将AVPacket数据发送给解码器进行解码
av_read_frame              // 读取码流中的音频若干帧或者视频一帧
avcodec_receive_frame

* 使用到的FFmpeg重要结构体
AVFormatContext
AVInputFormat
AVOutputFormat 

AVStream
AVPacket
AVFrame

AVCodecContext       // 解码器上下文
AVCodec              // 解码器
AVCodecParameters

* 使用的SDL重要函数
SDL_OpenAudio

* PCM采样格式
AV_SAMPLE_FMT_S16

* 一些时间获取函数
int64_t av_gettime()           //从公元1970年1月1日0时0分0秒开始的微秒值
int64_t av_gettime_relative()  // 返回开机后开始的微妙值



