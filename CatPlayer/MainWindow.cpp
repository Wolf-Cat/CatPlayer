#include "MainWindow.h"
#include "ui_Mainwindow.h"
#include "AvPlayDef.h"
#include "MyPlayer.h"

#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::Init()
{
    m_mediaCenter.Init();
    Global::GetInstance().label = ui->videoLabel;
    connect(&Global::GetInstance(), &Global::SigUpdateImage, this, &MainWindow::UpdateImage, Qt::QueuedConnection);
    QTimer::singleShot(20, this, [=]() {
        RefreshVideo();
    });
}

void MainWindow::RefreshVideo()
{
    VideoFrame vFrame;
    // vFrame.frame = av_frame_alloc();
    int idiff = 5;
    if (m_mediaCenter.m_pPlayer->m_decodeVFrameQue.GetFramePic(vFrame))
    {
        if (vFrame.curClock + vFrame.duration < m_mediaCenter.m_pPlayer->m_audioClock) {
            ConvertToImage(vFrame.frame);
        } else {
            double diff = vFrame.curClock + vFrame.duration - m_mediaCenter.m_pPlayer->m_audioClock;
            idiff = int(diff * 1000);
        }
    }

    QTimer::singleShot(idiff, this, [=]() {
        RefreshVideo();
    });
}

void MainWindow::UpdateImage(QImage img)
{
    static int i = 0;
    QPixmap pixmap = QPixmap::fromImage(img);
    ui->videoLabel->setPixmap(pixmap);
    //img.save(QString("./a%1.png").arg(i++),"PNG", 100);
}

void MainWindow::ConvertToImage(AVFrame *pFrame)
{
    if (pFrame == NULL) {
        qDebug() << "video frame is null";
        return;
    }

    SwsContext *swsContext = sws_getContext(
           pFrame->width,
           pFrame->height,
           AV_PIX_FMT_YUV420P, // 源像素格式
                pFrame->width,
                pFrame->height,
                AV_PIX_FMT_RGB24,   // 目标像素格式
                SWS_BICUBIC,
                NULL,
                NULL,
                NULL
            );

    if (swsContext == NULL) {
        return;
    }

    // 分配目标图像的内存
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pFrame->width, pFrame->height);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    AVPicture dstPic;
    avpicture_fill((AVPicture *)&dstPic, buffer, AV_PIX_FMT_RGB24, pFrame->width, pFrame->height);

    // 执行转换
    int ret = sws_scale(swsContext, (const uint8_t *const *)pFrame->data, pFrame->linesize, 0, pFrame->height, dstPic.data, dstPic.linesize);

    // 转换完成后释放SwsContext
    sws_freeContext(swsContext);

    QImage image((uchar*)dstPic.data[0], pFrame->width, pFrame->height, QImage::Format_RGB888);

    if (image.isNull()) {
        QString retErr = QString(av_myerr2str(ret));
        qDebug() << "Image is NULL";
    } else {
        static int frameNum = 0;
        frameNum++;
        qDebug() << "num Image frame num: " << frameNum;   // 目前能渲染180帧 Image, 宽高修改一半后也是一样, 跟QImage分配次数有关
    }

    QPixmap pixmap = QPixmap::fromImage(image);
    ui->videoLabel->setPixmap(pixmap);

    av_free(buffer);
    av_frame_unref(pFrame);
}

MainWindow::~MainWindow()
{
    delete ui;
}
