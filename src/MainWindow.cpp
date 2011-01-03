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
#include <QMessageBox>
#include <QFileDialog>

#include "AboutWidget.h"
#include "TimerView.h"
#include "Tournament.h"

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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tourneyName->setText(tr("Torneo del %1 ore %2").arg(QDate::currentDate().toString("d-M")).arg(QTime::currentTime().toString("h:m")));
    ui->tourneyName->selectAll();
    ui->tableLevels->setColumnWidth(0, 125);
    ui->tableLevels->setColumnWidth(1, 125);
    ui->tableLevels->setColumnWidth(2, 125);
    ui->tableLevels->setColumnWidth(3, 125);
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    // input validation (prevents segfaults)
    connect(ui->tourneyName, SIGNAL(textChanged(QString)), this, SLOT(validateStart()));
    connect(ui->tableLevels, SIGNAL(cellChanged(int,int)), this, SLOT(validateStart()));
    connect(ui->spinGiocatori, SIGNAL(valueChanged(int)), this, SLOT(validateStart()));
    connect(ui->spinChips, SIGNAL(valueChanged(int)), this, SLOT(validateStart()));
    connect(ui->spinRebuyLev, SIGNAL(valueChanged(int)), this, SLOT(validateStart()));
    connect(ui->spinRebuyChips, SIGNAL(valueChanged(int)), this, SLOT(validateStart()));

    m_about_widget = new AboutWidget;

    m_appsettings = new QSettings("settings.ini", QSettings::IniFormat, this);
    if(m_appsettings->contains("MainWindowGeometry"))
        this->setGeometry(m_appsettings->value("MainWindowGeometry").toRect());
}

MainWindow::~MainWindow()
{
    m_appsettings->setValue("MainWindowGeometry", geometry());

    delete ui;
    delete m_about_widget;
}

void MainWindow::on_actionAbout_triggered()
{
    int pos_x = frameGeometry().x() + (frameGeometry().width() - m_about_widget->frameGeometry().width()) / 2;
    int pos_y = frameGeometry().y() + (frameGeometry().height() - m_about_widget->frameGeometry().height()) / 2;

//    qDebug() << "New Pos is" << pos_x << pos_y;

    m_about_widget->move(pos_x, pos_y);

    if(m_about_widget->isVisible())
        m_about_widget->raise();
    else
        m_about_widget->show();
}

void MainWindow::on_actionNew_triggered()
{
    if (QMessageBox::question(this, "Nuovo Torneo", "Vuoi salvare il torneo attuale prima di cancellarlo?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
        on_actionSaveAs_triggered();
    }

    ui->tourneyName->clear();
    ui->tableLevels->clearContents();
    ui->tableLevels->setRowCount(0);
    ui->spinChips->setValue(0);
    ui->spinGiocatori->setValue(0);
    ui->spinRebuyChips->setValue(0);
    ui->spinRebuyLev->setValue(0);
}

void MainWindow::on_actionOpen_triggered()
{
    on_actionNew_triggered();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Apri"), QDir::homePath(), tr("Poker Tournament (*.pkt)"));
    if(fileName.isNull())
        return;

    QFile file(fileName);
    if(! (file.exists() && file.open(QIODevice::ReadOnly))) {
        qDebug() << "Wrong file!";
        return;
    }

    QDomDocument doc("xml-pokertimer");
    doc.setContent(&file);

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
    ui->spinGiocatori->setValue(QVariant(node_players.attributes().namedItem("number").nodeValue()).toInt());
    ui->spinChips->setValue(QVariant(node_players.attributes().namedItem("chipseach").nodeValue()).toInt());

    QDomNode node_rebuy = node_tournament.childNodes().at(2);
    ui->spinRebuyLev->setValue(QVariant(node_rebuy.attributes().namedItem("maxlevel").nodeValue()).toInt());
    ui->spinRebuyChips->setValue(QVariant(node_rebuy.attributes().namedItem("chips").nodeValue()).toInt());

    //qDebug() << doc.toString();
}

void MainWindow::on_actionSave_triggered()
{
    on_actionSaveAs_triggered();
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Salva come..."), QDir::homePath(), tr("Poker Tournament (*.pkt)"));
    if(fileName.isNull())
        return;

    QDomDocument doc("xml-pokertimer");
    QDomElement el_tournament = doc.createElement("tournament");
    el_tournament.setAttribute("name", ui->tourneyName->text());
    doc.appendChild(el_tournament);

    QDomElement el_levels = doc.createElement("levels");
    el_tournament.appendChild(el_levels);

    for(int i = 0; i < ui->tableLevels->rowCount(); i++) {
        QDomElement el_lev = doc.createElement("level");
        el_lev.setAttribute("time", ui->tableLevels->item(i,0)->text());
        el_lev.setAttribute("ante", ui->tableLevels->item(i,1)->text());
        el_lev.setAttribute("smallblind", ui->tableLevels->item(i,2)->text());
        el_lev.setAttribute("bigblind", ui->tableLevels->item(i,3)->text());
        el_levels.appendChild(el_lev);
    }

    QDomElement el_players = doc.createElement("players");
    el_players.setAttribute("number", QString::number(ui->spinGiocatori->value()));
    el_players.setAttribute("chipseach", QString::number(ui->spinChips->value()));
    el_tournament.appendChild(el_players);

    QDomElement el_rebuy = doc.createElement("rebuy");
    el_rebuy.setAttribute("maxlevel", QString::number(ui->spinRebuyLev->value()));
    el_rebuy.setAttribute("chips", QString::number(ui->spinRebuyChips->value()));
    el_tournament.appendChild(el_rebuy);

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(doc.toByteArray());
    file.close();
}

void MainWindow::on_addLevel_clicked()
{
    //qDebug() << ui->tableLevels->currentRow();
    if(ui->tableLevels->currentRow() >= 0 && ui->tableLevels->currentRow() < ui->tableLevels->rowCount() - 1) {
        ui->tableLevels->insertRow(ui->tableLevels->currentRow() + 1);
    }
    else {
        ui->tableLevels->setRowCount(ui->tableLevels->rowCount() + 1);
    }
}

void MainWindow::on_remLevel_clicked()
{
    if(ui->tableLevels->rowCount() > 0) {
        if(ui->tableLevels->currentRow() < 0) {
            ui->tableLevels->setRowCount(ui->tableLevels->rowCount() - 1);
        }
        else {
            ui->tableLevels->removeRow(ui->tableLevels->currentRow());
        }
    }
}

#if QT_VERSION >= 0x040600
#include <QPropertyAnimation>
#endif
void MainWindow::on_startButton_clicked()
{
    if (QMessageBox::question(this, "Inizia Torneo", "Dopo aver iniziato il torneo non potrai piu' modificare le impostazioni.\nContinuare?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
#if QT_VERSION >= 0x040600
        QWidget * pw = centralWidget();
        int hw = pw->width() / 2;
        int he = pw->height() / 2;
        QPropertyAnimation * ani = 0;
        for (int i = 0; i < ui->spinGiocatori->value() * 4; i++) {
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

void MainWindow::validateStart()
{
    bool ok = !ui->tourneyName->text().isEmpty()
           && ui->spinChips->value() > 0
           && ui->spinGiocatori->value() > 0
           //&& ui->spinRebuyChips->value() > 0
           //&& ui->spinRebuyLev->value() > 0
           && ui->tableLevels->rowCount() > 0;
    ui->startButton->setEnabled(ok);
}

void MainWindow::startTournament()
{
    Tournament *tournament = new Tournament(this);
    tournament->setName(ui->tourneyName->text());
    tournament->setCurrentPlayers(ui->spinGiocatori->value());
    tournament->setTotalPlayers(ui->spinGiocatori->value());
    tournament->setChipsEach(ui->spinChips->value());
    tournament->setRebuyMaxLevel(ui->spinRebuyLev->value());
    tournament->setRebuyChips(ui->spinRebuyChips->value());

    for (int i = 0; i < ui->tableLevels->rowCount(); i++) {
        Tournament::Level level;
        level.time_minutes = QVariant(ui->tableLevels->item(i,0)->text()).toInt();
        level.ante = QVariant(ui->tableLevels->item(i,1)->text()).toInt();
        level.smallblind = QVariant(ui->tableLevels->item(i,2)->text()).toInt();
        level.bigblind = QVariant(ui->tableLevels->item(i,3)->text()).toInt();
        tournament->addLevel(level);
        //qDebug() << "Livello" << i << "-" << level.time_minutes << level.ante << level.smallblind << level.bigblind;
    }

    TimerView *tv = new TimerView(tournament);
    tv->setAttribute(Qt::WA_DeleteOnClose);
    tv->setWindowState(Qt::WindowFullScreen);
    tv->show();
}
