/***************************************************************************
 *                                                                         *
 *   This file is part of the PokerTournamentManager project,              *
 *       http://www.gitorious.org/poker-tournament-manager                 *
 *                                                                         *
 *   Authors: Alessandro Costa <kost.on.linux@gmail.com>, (C) 2009         *
 *            Enrico Ros <enrico.ros@gmail.com>, (C) 2009                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation. For more details see the   *
 *   COPYING.txt file that comes with this distribution.                   *
 *                                                                         *
 ***************************************************************************/

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtXml>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QSettings>
#include <QTableWidgetItem>

#include "AboutWidget.h"
#include "Global.h"
#include "LevelEditDialog.h"
#include "TimerView.h"
#include "Tournament.h"

//#if QT_VERSION >= 0x040600
//#define CHIPS_ANIMATION_ON_START
//#endif

// ===== CLASS "Chip" =============================================================================

Chip::Chip(QWidget * parent)
  : QWidget(parent)
  , m_opacity(1.0)
{
    static QString names[5] = {"black", "blue", "green", "red", "white"};
    m_pix = QPixmap(":/data/chip-" + names[qrand() % 5] + ".png");
    setFixedSize(48, 48);
    show();
}

qreal Chip::opacity() const
{
    return m_opacity;
}

void Chip::setOpacity(qreal value)
{
    m_opacity = value;
    update();
}

void Chip::paintEvent(QPaintEvent * /*event*/)
{
    QPainter p(this);
    if (m_opacity < 1.0)
        p.setOpacity(m_opacity);
    p.drawPixmap(0, 0, m_pix);
}

// ===== CLASS "MainWindow" =======================================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_tournament(0)
{
    ui->setupUi(this);
    ui->tableLevels->setColumnWidth(0, 75);
    ui->tableLevels->setColumnWidth(1, 75);
    ui->tableLevels->setColumnWidth(2, 110);
    ui->tableLevels->setColumnWidth(3, 110);
    ui->tableLevels->setColumnWidth(4, 110);
    ui->tableLevels->setColumnWidth(5, 75);

    // input validation (prevents segfaults)
    connect(ui->actionExit,     SIGNAL(triggered()),          this, SLOT(close()));
    connect(ui->tableLevels,    SIGNAL(cellChanged(int,int)), this, SLOT(validateStart()));

    connect(ui->tourneyName,    SIGNAL(textChanged(QString)), this, SLOT(validateStart()));
    connect(ui->spinPlayers,    SIGNAL(valueChanged(int)),    this, SLOT(validateStart()));
    connect(ui->spinChips,      SIGNAL(valueChanged(int)),    this, SLOT(validateStart()));
    connect(ui->spinRebuyChips, SIGNAL(valueChanged(int)),    this, SLOT(validateStart()));

    connect(ui->tourneyName,    SIGNAL(editingFinished()), this, SLOT(handleNameEdit()));
    connect(ui->spinChips,      SIGNAL(editingFinished()), this, SLOT(handleChipsEdit()));
    connect(ui->spinPlayers,    SIGNAL(editingFinished()), this, SLOT(handlePlayersEdit()));
    connect(ui->spinRebuyChips, SIGNAL(editingFinished()), this, SLOT(handleRebuyChipsEdit()));

    m_about_widget = new AboutWidget;
    m_level_editor = new LevelEditDialog;

    m_appsettings = new QSettings("settings.ini", QSettings::IniFormat, this);
    if(m_appsettings->contains("MainWindowGeometry"))
        this->setGeometry(m_appsettings->value("MainWindowGeometry").toRect());

    on_actionNew_triggered();
}

MainWindow::~MainWindow()
{
    m_appsettings->setValue("MainWindowGeometry", geometry());

    delete ui;
    delete m_about_widget;
}

void MainWindow::handleChipsEdit()
{
    if(! m_tournament)
        return;

    m_tournament->setInitialStack(ui->spinChips->value());
    qDebug() << "Tournament Chips Each:" << m_tournament->initialStack();
    validateStart();
}

void MainWindow::handleNameEdit()
{
    if(! m_tournament)
        return;

    m_tournament->setName(ui->tourneyName->text());
    qDebug() << "Tournament Name:" << m_tournament->name();
    validateStart();
}

void MainWindow::handlePlayersEdit()
{
    if(! m_tournament)
        return;

    m_tournament->setTotalPlayers(ui->spinPlayers->value());
    qDebug() << "Tournament Players:" << m_tournament->totalPlayers();
    validateStart();
}

void MainWindow::handleRebuyChipsEdit()
{
    if(! m_tournament)
        return;

    m_tournament->setRebuyStack(ui->spinRebuyChips->value());
    qDebug() << "Tournament Rebuy Chips:" << m_tournament->rebuyStack();
    validateStart();
}

void MainWindow::on_actionAbout_triggered()
{
    int pos_x = frameGeometry().x() + (frameGeometry().width() - m_about_widget->frameGeometry().width()) / 2;
    int pos_y = frameGeometry().y() + (frameGeometry().height() - m_about_widget->frameGeometry().height()) / 2;

    m_about_widget->move(pos_x, pos_y);

    if(m_about_widget->isVisible())
        m_about_widget->raise();
    else
        m_about_widget->show();
}

void MainWindow::on_actionNew_triggered()
{
    if(m_tournament)
    {
        if(QMessageBox::question(this, tr("Nuovo Torneo"), tr("Vuoi salvare il torneo attuale prima di cancellarlo?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
            on_actionSave_triggered();
        }

        delete m_tournament;
        m_tournament = 0;
    }

    m_tournament = new Tournament;
    m_tournament->setName(tr("Torneo del %1 ore %2").arg(QDate::currentDate().toString("d-M")).arg(QTime::currentTime().toString("h:m")));
    updateGraphics();
}

void MainWindow::on_actionOpen_triggered()
{
    if(m_tournament)
    {
        if(QMessageBox::question(this, tr("Apri"), tr("Vuoi salvare il torneo attuale prima di cancellarlo?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
            on_actionSaveAs_triggered();
        }

        delete m_tournament;
        m_tournament = 0;
    }

    QString filename = QFileDialog::getOpenFileName(this, tr("Apri"), QDir::homePath(), tr("Poker Tournament (*.pkt)"));
    if(filename.isNull() || filename.isEmpty())
    {
        on_actionNew_triggered();
        return;
    }

    QFile file(filename);
    if(! (file.exists() && file.open(QIODevice::ReadOnly)))
    {
        QMessageBox::warning(this, tr("Apri"), tr("Impossibile aprire il file."), QMessageBox::Ok);
        on_actionNew_triggered();
        return;
    }

    QDomDocument doc;
    doc.setContent(&file);

    if(doc.doctype().name() != PTM_XML_DOCTYPE)
    {
        QMessageBox::warning(this, tr("Apri"), tr("Errore nel formato del file."), QMessageBox::Ok);
        on_actionNew_triggered();
        return;
    }

    m_tournament = new Tournament;
    if(! m_tournament->fromXml(doc))
    {
        QMessageBox::warning(this, tr("Apri"), tr("Errore nella lettura del file."), QMessageBox::Ok);

        delete m_tournament;
        m_tournament = 0;

        on_actionNew_triggered();
        return;
    }

    m_filename = filename;
    updateGraphics();
    return;

    QDomNode node_tournament = doc.childNodes().at(0);
    ui->tourneyName->setText(node_tournament.attributes().namedItem("name").nodeValue());

    QDomNode node_levels = node_tournament.childNodes().at(0);
    for(int i = 0; i < node_levels.childNodes().size(); i++) {
        QDomNode node_lev = node_levels.childNodes().at(i);
        ui->tableLevels->setRowCount(i+1);

        ui->tableLevels->setItem(i, 0, new QTableWidgetItem(""));
        ui->tableLevels->setItem(i, 1, new QTableWidgetItem(""));
        ui->tableLevels->setItem(i, 2, new QTableWidgetItem(""));
        ui->tableLevels->setItem(i, 3, new QTableWidgetItem(""));

        ui->tableLevels->item(i, 0)->setText(node_lev.attributes().namedItem("time").nodeValue());
        ui->tableLevels->item(i, 1)->setText(node_lev.attributes().namedItem("ante").nodeValue());
        ui->tableLevels->item(i, 2)->setText(node_lev.attributes().namedItem("smallblind").nodeValue());
        ui->tableLevels->item(i, 3)->setText(node_lev.attributes().namedItem("bigblind").nodeValue());

        //qDebug() << node_lev.attributes().namedItem("time").nodeValue() << node_lev.attributes().namedItem("ante").nodeValue() << node_lev.attributes().namedItem("smallblind").nodeValue() << node_lev.attributes().namedItem("bigblind").nodeValue();
    }

    QDomNode node_players = node_tournament.childNodes().at(1);
    ui->spinPlayers->setValue(QVariant(node_players.attributes().namedItem("number").nodeValue()).toInt());
    ui->spinChips->setValue(QVariant(node_players.attributes().namedItem("chipseach").nodeValue()).toInt());

    QDomNode node_rebuy = node_tournament.childNodes().at(2);
//    ui->spinRebuyLev->setValue(QVariant(node_rebuy.attributes().namedItem("maxlevel").nodeValue()).toInt());
    ui->spinRebuyChips->setValue(QVariant(node_rebuy.attributes().namedItem("chips").nodeValue()).toInt());

    //qDebug() << doc.toString();
}

void MainWindow::on_actionSave_triggered()
{
    if(m_filename.isNull() || m_filename.isEmpty())
    {
        on_actionSaveAs_triggered();
        return;
    }

    QDomDocument doc(PTM_XML_DOCTYPE);
    m_tournament->toXml(doc);

    QFile file(m_filename);
    file.open(QIODevice::WriteOnly);
    file.write(doc.toByteArray());
    file.close();
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Salva come..."), QDir::homePath(), tr("Poker Tournament (*.pkt)"));
    if(fn.isNull() || fn.isEmpty())
        return;

    m_filename = fn;
    if(! m_filename.endsWith(".pkt"))
        m_filename.append(".pkt");

    on_actionSave_triggered();
}

void MainWindow::on_levelAdd_clicked()
{
    int pos = ui->tableLevels->currentRow();

    Level* l = m_tournament->addLevel(pos);
    m_level_editor->setLevel(l, pos < 0 ? m_tournament->countLevels() : pos + 1);
    if(! m_level_editor->exec())
    {
        m_tournament->removeLevel(pos);
    }

    updateGraphics();
}

void MainWindow::on_levelDown_clicked()
{
    if(ui->tableLevels->currentRow() < 0 || ui->tableLevels->currentRow() == ui->tableLevels->rowCount() - 1)
        return;

    int row = ui->tableLevels->currentRow();
    m_tournament->moveLevelDown(row);
    updateGraphics();
    ui->tableLevels->setFocus();
    ui->tableLevels->selectRow(row + 1);
}

void MainWindow::on_levelEdit_clicked()
{
    if(ui->tableLevels->currentRow() < 0)
        return;

    Level* l = m_tournament->level(ui->tableLevels->currentRow());
    m_level_editor->setLevel(l, ui->tableLevels->currentRow() + 1);
    if(m_level_editor->exec())
    {
        updateGraphics();
    }
}

void MainWindow::on_levelRem_clicked()
{
    if(m_tournament->countLevels() <= 0)
        return;

    m_tournament->removeLevel(ui->tableLevels->currentRow());
    updateGraphics();
}

void MainWindow::on_levelUp_clicked()
{
    if(ui->tableLevels->currentRow() <= 0)
        return;

    int row = ui->tableLevels->currentRow();
    m_tournament->moveLevelUp(row);
    updateGraphics();
    ui->tableLevels->setFocus();
    ui->tableLevels->selectRow(row - 1);
}

#ifdef CHIPS_ANIMATION_ON_START
#include <QPropertyAnimation>
#endif
void MainWindow::on_startButton_clicked()
{
    ui->tableLevels->setFocus();

    if (QMessageBox::question(this, "Inizia Torneo", "Dopo aver iniziato il torneo non potrai piu' modificare le impostazioni.\nContinuare?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
#ifdef CHIPS_ANIMATION_ON_START
        QWidget * pw = centralWidget();
        int hw = pw->width() / 2;
        int he = pw->height() / 2;
        QPropertyAnimation * ani = 0;
        for (int i = 0; i < ui->spinPlayers->value() * 4; i++) {
            Chip * chip = new Chip(pw);
            chip->move(ui->startButton->geometry().center() + QPoint(qrand() % 100 - 50, qrand() % 40 - 20));
            ani = new QPropertyAnimation(chip, "pos", this);
            ani->setEasingCurve(QEasingCurve::OutQuad);
            ani->setDuration(1000);
            ani->setEndValue(QPoint(qrand() % hw + qrand() % hw - 24, qrand() % he - 24));
            ani->start(QPropertyAnimation::DeleteWhenStopped);
            ani = new QPropertyAnimation(chip, "opacity", this);
            ani->setEasingCurve(QEasingCurve::OutQuad);
            ani->setDuration(1000);
            ani->setEndValue(0.0);
            ani->start(QPropertyAnimation::DeleteWhenStopped);
        }
        QTimer::singleShot(1200, this, SLOT(startTournament()));
#else
        startTournament();
#endif
    }
}

void MainWindow::updateGraphics(bool update_levels)
{
    ui->tourneyName->setText(m_tournament->name());
    ui->spinChips->setValue(m_tournament->initialStack());
    ui->spinPlayers->setValue(m_tournament->totalPlayers());
    ui->spinRebuyChips->setValue(m_tournament->rebuyStack());
//    ui->spinRebuyLev->setValue(m_tournament->rebuyMaxLevel());

    ui->tableLevels->clearContents();
    ui->tableLevels->setRowCount(0);

    if(update_levels)
    {
        ui->tableLevels->setRowCount(m_tournament->countLevels());

        for(int i = 0; i < m_tournament->countLevels(); i++)
        {
            Level *l = m_tournament->level(i);
            QTableWidgetItem* twi = 0;

            if(l)
            {
                twi = new QTableWidgetItem(l->strType());
                twi->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->tableLevels->setItem(i, 0, twi);

                twi = new QTableWidgetItem(l->time().toString("mm:ss"));
                twi->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->tableLevels->setItem(i, 1, twi);

                if(l->type() == Level::GameLevel)
                {
                    twi = new QTableWidgetItem(QString("%1").arg(l->ante()));
                    twi->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                    ui->tableLevels->setItem(i, 2, twi);

                    twi = new QTableWidgetItem(QString("%1").arg(l->smallBlind()));
                    twi->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                    ui->tableLevels->setItem(i, 3, twi);

                    twi = new QTableWidgetItem(QString("%1").arg(l->bigBlind()));
                    twi->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                    ui->tableLevels->setItem(i, 4, twi);

                    twi = new QTableWidgetItem(l->isRebuyEnabled()? tr("SI") : tr("NO"));
                    twi->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    ui->tableLevels->setItem(i, 5, twi);
                }
            }
        }
    }
}

void MainWindow::validateStart()
{
    bool ok =  !ui->tourneyName->text().isEmpty()
               && ui->spinChips->value() > 0
               && ui->spinPlayers->value() > 0
               //&& ui->spinRebuyChips->value() > 0
               //&& ui->spinRebuyLev->value() > 0
               && ui->tableLevels->rowCount() > 0;

    ui->startButton->setEnabled(ok);
}

void MainWindow::startTournament()
{
    m_tournament->start();

    TimerView *tv = new TimerView(m_tournament);
    tv->setAttribute(Qt::WA_DeleteOnClose);
    tv->setWindowState(Qt::WindowFullScreen);
    tv->show();

    m_tournament = 0;
    on_actionNew_triggered();
}
