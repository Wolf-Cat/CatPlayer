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
    Global::GetInstance().m_label = ui->videoLabel;
    m_mediaCenter.Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}
