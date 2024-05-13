#include "CtrlBar.h"
#include "ui_CtrlBar.h"

CtrlBar::CtrlBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CtrlBar)
{
    ui->setupUi(this);
}

CtrlBar::~CtrlBar()
{
    delete ui;
}
