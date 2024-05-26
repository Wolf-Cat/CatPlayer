#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MediaCentre.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void Init();
    void UpdateImage(QImage img);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    MediaCentre m_mediaCenter;
};

#endif // MAINWINDOW_H
