#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MediaCentre.h"

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
    ~MainWindow();

    MediaCentre m_mediaCenter;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
