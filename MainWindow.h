#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <QSpinBox>
#include <QTableWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        QSpinBox *m_sbox_chips;
        QSpinBox *m_sbox_ngiocatori;
        QSpinBox *m_sbox_rebuychips;
        QSpinBox *m_sbox_rebuylev;
        QTableWidget *m_table_levels;
        QLineEdit *m_txt_name;

    private slots:
        void addLevelClicked();
        void exitTriggered();
        void newTriggered();
        void openTriggered();
        void remLevelClicked();
        void saveTriggered();
        void saveAsTriggered();
        void startClicked();
};

#endif // MAINWINDOW_H
