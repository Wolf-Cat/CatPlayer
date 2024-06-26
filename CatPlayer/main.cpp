#include "MainWindow.h"
#include <QApplication>
#include <QDebug>

extern "C"
{

#include "libavutil/avutil.h"
#include "SDL.h"
#undef main      // 非常重要，用以区分QTmain函数入口和SDL2main函数入口

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    w.Init();

    SDL_version sdlVer;
    SDL_VERSION(&sdlVer);

    qDebug() << "FFmpeg的版本：" << av_version_info();
    qDebug() << "SDL的版本：" << sdlVer.major << " " << sdlVer.minor;

    int nSoundDevice = SDL_GetNumAudioDevices(0);

    for (int i = 0; i < nSoundDevice; i++) {
       printf("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
    }

    return a.exec();
}
