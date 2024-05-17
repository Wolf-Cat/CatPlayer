#include "MainWindow.h"
#include "ui_Mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::Init()
{
    m_mediaCenter.Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}
