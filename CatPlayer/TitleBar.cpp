#include "TitleBar.h"
#include "ui_Titlebar.h"

TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleBar)
{
    ui->setupUi(this);
}

TitleBar::~TitleBar()
{
    delete ui;
}
