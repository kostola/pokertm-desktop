#include "AboutWidget.h"
#include "ui_AboutWidget.h"

#include "Global.h"

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWidget)
{
    ui->setupUi(this);
    ui->txt_version->setText(PTM_VERSION);
}

AboutWidget::~AboutWidget()
{
    delete ui;
}
