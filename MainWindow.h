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
};

#endif // MAINWINDOW_H
