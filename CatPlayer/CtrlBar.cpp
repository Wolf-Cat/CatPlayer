#include "CtrlBar.h"
#include "ui_CtrlBar.h"

CtrlBar::CtrlBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CtrlBar)
{
    ui->setupUi(this);
    InitConnect();
}

CtrlBar::~CtrlBar()
{
    delete ui;
}

void CtrlBar::InitConnect()
{
    connect(ui->btnPlayPause, &QPushButton::clicked, this, &CtrlBar::OnBtnPlayPauseClicked);
}

void CtrlBar::OnBtnPlayPauseClicked()
{

}
