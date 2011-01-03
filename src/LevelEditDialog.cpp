#include "LevelEditDialog.h"
#include "ui_LevelEditDialog.h"

#include <QDebug>

LevelEditDialog::LevelEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LevelEditDialog)
    , m_level(0)
{
    ui->setupUi(this);

    ui->cboxType->addItem(tr("GIOCO"), 0);
    ui->cboxType->addItem(tr("PAUSA"), 1);
}

LevelEditDialog::~LevelEditDialog()
{
    delete ui;
}

void LevelEditDialog::on_pbCancel_clicked()
{
    this->done(0);
}

void LevelEditDialog::on_pbOk_clicked()
{
    if(! m_level)
        return;

    m_level->setTime(ui->timeEdit->time());

    if(ui->cboxType->currentText() == tr("GIOCO"))
    {
        m_level->setType(Level::GameLevel);
        m_level->setAnte(ui->sboxAnte->value());
        m_level->setBigBlind(ui->sboxBigblind->value());
        m_level->setSmallBlind(ui->sboxSmallblind->value());
        m_level->setRebuyEnabled(ui->cboxRebuy->isChecked());
    }
    else if(ui->cboxType->currentText() == tr("PAUSA"))
    {
        m_level->setType(Level::PauseLevel);
    }

    this->done(1);
}

void LevelEditDialog::setLevel(Level *l, int number)
{
    m_level = l;
    ui->txtNumber->setText(QString("%1").arg(number));

    ui->cboxType->setCurrentIndex(l->type() == Level::GameLevel ? 0 : 1);
    ui->timeEdit->setTime(l->time());
    ui->sboxAnte->setValue(l->ante());
    ui->sboxBigblind->setValue(l->bigBlind());
    ui->sboxSmallblind->setValue(l->smallBlind());
    ui->cboxRebuy->setChecked(l->isRebuyEnabled());
}
