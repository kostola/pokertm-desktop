#ifndef LEVELEDITDIALOG_H
#define LEVELEDITDIALOG_H

#include <QDialog>

#include "Tournament.h"

namespace Ui {
    class LevelEditDialog;
}

class LevelEditDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit LevelEditDialog(QWidget *parent = 0);
        ~LevelEditDialog();

        void setLevel(Level* l, int number);

    private:
        Ui::LevelEditDialog *ui;
        Level * m_level;

    private slots:
        void on_pbCancel_clicked();
        void on_pbOk_clicked();
};

#endif // LEVELEDITDIALOG_H
