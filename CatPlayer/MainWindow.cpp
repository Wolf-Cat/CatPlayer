#include "MainWindow.h"
#include "ui_Mainwindow.h"
#include "AvPlayDef.h"

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
}

void MainWindow::UpdateImage(QImage img)
{
    static int i = 0;
    QPixmap pixmap = QPixmap::fromImage(img);
    ui->videoLabel->setPixmap(pixmap);
    //img.save(QString("./a%1.png").arg(i++),"PNG", 100);
}

MainWindow::~MainWindow()
{
    delete ui;
}
