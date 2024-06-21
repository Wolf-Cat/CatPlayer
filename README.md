# CatPlayer
一只猫的播放器

* 播放器架构组成
界面UI模块
桥接控制模块 : 用于界面层和播放器之间的消息传递，采用消息队列的方式
播放器模块MyPlayer : 读取音视频数据，解码，播放，音视频同步

* 主要线程
1. 数据读取解复用线程
2. 视频解码线程
3. 音频解码，音频播放线程    （由SDL在打开音频设备时创建回调）
5. 视频渲染放在主线程里面做


* 主要队列
1. 界面层和播放器之间的消息队列 （起到桥接作用）
2. 视频AVPacket队列
3. 视频Frame队列
4. 音频AVPacket队列
5. 音频Frame队列

* 音视频同步
以音频为基准，视频向音频同步
视频在渲染前拿到这一帧的pts，与aduio_clock对比，从而决定是播放还是延迟播放
视频时钟小于音频时钟，立刻渲染视频，反之，等待差值后再渲染

* 使用到的FFmpeg重要的函数
avformat_open_input        // 打开文件，如果是网络文件则创建网络链接等
avformat_find_stream_info  // 检索流信息
avcodec_open2              // 将解码器绑定至解码器上下文 Initialize the AVCodecContext to use the given AVCodec 
avcodec_send_packet        // 将AVPacket数据发送给解码器进行解码
av_read_frame              // 读取码流中的音频若干帧或者视频一帧
avcodec_receive_frame      // 从解码器上下文中不断去读取解码后的帧
avcodec_decode_video2      // 将视频包解码成视频帧
av_packet_unref            // 

swr_convert    // 音频重采样

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

SwsContext     // 图像转换上下文，音频重采样上下文

* 使用的SDL重要函数
SDL_OpenAudio    // 打开扬声器，会创建音频播放线程，调用回调函数

* PCM采样格式
AV_SAMPLE_FMT_S16

* 一些时间获取函数
int64_t av_gettime()           //从公元1970年1月1日0时0分0秒开始的微秒值
int64_t av_gettime_relative()  // 返回开机后开始的微妙值

* FFmpeg的架构组成
libavcodec：编解码库。
libavformat：音视频容器格式以及所支持的协议的封装和解析。
libavutil：提供了一些公共函数，工具库。
libavfilter：音视频的滤镜库，如视频加水印、音频变声等。
libavdevice：支持众多设备数据的输入与输出，如读取摄像头数据、屏幕录制。
libswresample, libavresample：提供音频的重采样工具库。
libswscale：提供对视频图像进行色彩转换、缩放以及像素格式转换，如图像的 YUV 转换。
libpostproc：多媒体后处理器。

* 总结的一些经验教训
1. 分配和释放AVPacket和AVFrame，尽量用FFmpeg自带的av_packet_alloc/av_packet_unref/av_packet_move_unref   
   av_frame/av_frame_unref 这些，不要使用new/malloc去自动分配

2. 尽量不要直接对AVFrame/AVPacket指针去相互赋值，应该使用 av_frame_move_ref
  
3. SDL_OpenAudio(&wantedSpec, NULL); 第二个参数设置为NULL, 才会按照请求的参数设置扬声器
 官方解释
  If \c obtained is NULL, the audio
 *  data passed to the callback function will be guaranteed to be in the
 *  requested format

4. avcodec_receive_frame 类的函数返回AVERROR(EAGAIN)为-11，意味着需要新的输入数据才能返回新的输出
   在解码或编码时，编解码器可能会接收到多个输入帧/数据包而不返回帧，直到其内部缓冲区被填充为止

5. 在进行音视频同步的时候AVFrame的pts字段对应AVStream的time_base时间基

6. Qt中是无法直接显示YUV数据的，所以必须把YUV数据转换为RGB32再通过QImage显示到控件中

7. QImage内部是动态申请内存的，所以如果一直不停申请的话，是会出现QImage: out of memory, returning null image错误的
