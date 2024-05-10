#include "Mainwindow.h"
#include <QApplication>
#include <QDebug>

extern "C"
{
#include "libavutil/avutil.h"
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qDebug() << "FFmpeg的版本：" << av_version_info();

    return a.exec();
}
