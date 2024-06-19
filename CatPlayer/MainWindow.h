#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "MediaCentre.h"

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace Ui {
class MainWindow;
}

class AVFrame;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void Init();
    void RefreshVideo();
    void UpdateImage(QImage img);
    void ConvertToImage(AVFrame *pFrame);
    void SlotSigImgData(uchar *data, int width, int height);
    void SlotAVPicDisplay(uint8_t *pic, int width, int height);
    ~MainWindow();

    MediaCentre m_mediaCenter;
private:
    Ui::MainWindow *ui;
    QImage m_img;
};

#endif // MAINWINDOW_H
