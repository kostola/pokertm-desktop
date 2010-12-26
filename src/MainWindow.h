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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <QSpinBox>
#include <QTableWidget>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = 0);

    private:
        Ui::MainWindow ui;

    private slots:
        void on_actionNew_triggered();
        void on_actionOpen_triggered();
        void on_actionSave_triggered();
        void on_actionSaveAs_triggered();
        void on_addLevel_clicked();
        void on_remLevel_clicked();
        void on_startButton_clicked();
        void validateStart();
        void startTournament();
};

class Chip : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    public:
        Chip(QWidget * parent);

    public slots:
        qreal opacity() const;
        void setOpacity(qreal value);

    protected:
        void paintEvent(QPaintEvent * /*event*/);

    private:
        QPixmap m_pix;
        qreal m_opacity;
};

#endif // MAINWINDOW_H
