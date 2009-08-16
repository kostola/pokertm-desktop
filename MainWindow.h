#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        QTableWidget *table_levels;

    private slots:
        void addLevelClicked();
        void remLevelClicked();
        void startClicked();
};

#endif // MAINWINDOW_H
