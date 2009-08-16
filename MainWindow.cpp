#include "MainWindow.h"

#include <QtGui>

#include "TimerView.h"
#include "Tournament.h"

QGroupBox* wrapInGroupBox(QLayout *l, const QString& title) {
    QGroupBox *gb = new QGroupBox(title);
    gb->setLayout(l);
    return gb;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar *menu_bar = new QMenuBar();
    QMenu *menu_file = menu_bar->addMenu(tr("&File"));
    QAction *action_new = menu_file->addAction(tr("&Nuovo"));
    QAction *action_open = menu_file->addAction(tr("&Apri"));
    QAction *action_save = menu_file->addAction(tr("&Salva"));
    QAction *action_saveas = menu_file->addAction(tr("Salva &come.."));
    menu_file->addSeparator();
    QAction *action_exit = menu_file->addAction(tr("&Esci"));
    this->setMenuBar(menu_bar);

    // composizione widget centrale
    QLineEdit *txt_name = new QLineEdit();
    QHBoxLayout *lay_name = new QHBoxLayout();
    lay_name->addWidget(txt_name);
    QGroupBox *gbox_name = wrapInGroupBox(lay_name, tr("Nome Torneo"));

    QStringList header_labels;
    header_labels << "Ante" << "Piccolo Buio" << "Grande Buio";

    table_levels = new QTableWidget();
    table_levels->setMinimumWidth(500);
    table_levels->setColumnCount(3);
    table_levels->setRowCount(0);
    table_levels->setHorizontalHeaderLabels(header_labels);
    table_levels->setColumnWidth(0, 170);
    table_levels->setColumnWidth(1, 170);
    table_levels->setColumnWidth(2, 170);

    QPushButton *pb_add_level = new QPushButton("+");
    pb_add_level->setFont(QFont("Helvetica", 16, QFont::Bold));
    pb_add_level->setFixedSize(30,30);
    QPushButton *pb_rem_level = new QPushButton("-");
    pb_rem_level->setFont(QFont("Helvetica", 16, QFont::Bold));
    pb_rem_level->setFixedSize(30,30);

    QObject::connect(pb_add_level, SIGNAL(clicked()), this, SLOT(addLevelClicked()));
    QObject::connect(pb_rem_level, SIGNAL(clicked()), this, SLOT(remLevelClicked()));

    QVBoxLayout *lay_levels_button = new QVBoxLayout();
    lay_levels_button->addWidget(pb_add_level);
    lay_levels_button->addWidget(pb_rem_level);
    lay_levels_button->addStretch(10);

    QHBoxLayout *lay_levels = new QHBoxLayout();
    lay_levels->addWidget(table_levels);
    lay_levels->addLayout(lay_levels_button);

    QGroupBox *gbox_levels = wrapInGroupBox(lay_levels, tr("Gestione Livelli"));

    QLabel *lb1 = new QLabel("Giocatori:");
    QLabel *lb2 = new QLabel("Chips a testa:");

    QSpinBox *sbox_ngiocatori = new QSpinBox();
    sbox_ngiocatori->setRange(0, 100000);
    sbox_ngiocatori->setMinimumWidth(160);
    QSpinBox *sbox_chips = new QSpinBox();
    sbox_chips->setRange(0, 10000000);
    sbox_chips->setMinimumWidth(160);

    QHBoxLayout *lay_chips = new QHBoxLayout();
    lay_chips->addWidget(lb1, Qt::AlignRight);
    lay_chips->addWidget(sbox_ngiocatori);
    lay_chips->addStretch(5);
    lay_chips->addWidget(lb2, Qt::AlignRight);
    lay_chips->addWidget(sbox_chips);

    QGroupBox *gbox_chips = wrapInGroupBox(lay_chips, tr("Gestione Chips"));

    QLabel *lb3 = new QLabel("Livello:");
    QLabel *lb4 = new QLabel("Chips:");

    QSpinBox *sbox_rebuylev = new QSpinBox();
    sbox_rebuylev->setRange(0, 100000);
    sbox_rebuylev->setMinimumWidth(160);
    QSpinBox *sbox_rebuychips = new QSpinBox();
    sbox_rebuychips->setRange(0, 10000000);
    sbox_rebuychips->setMinimumWidth(160);

    QHBoxLayout *lay_rebuy = new QHBoxLayout();
    lay_rebuy->addWidget(lb3, Qt::AlignRight);
    lay_rebuy->addWidget(sbox_rebuylev);
    lay_rebuy->addStretch(5);
    lay_rebuy->addWidget(lb4, Qt::AlignRight);
    lay_rebuy->addWidget(sbox_rebuychips);

    QGroupBox *gbox_rebuy = wrapInGroupBox(lay_rebuy, tr("Gestione Rebuy"));

    QPushButton *pb_start = new QPushButton(tr("S  T  A  R  T"));
    pb_start->setFont(QFont("Helvetica", 18, QFont::Normal));

    QObject::connect(pb_start, SIGNAL(clicked()), this, SLOT(startClicked()));

    QVBoxLayout *lay_central = new QVBoxLayout();
    lay_central->addWidget(gbox_name);
    lay_central->addWidget(gbox_levels);
    lay_central->addWidget(gbox_chips);
    lay_central->addWidget(gbox_rebuy);
    lay_central->addWidget(pb_start);

    QWidget *w_central = new QWidget();
    w_central->setLayout(lay_central);

    this->setCentralWidget(w_central);
    this->setMinimumSize(650, 520);
}

MainWindow::~MainWindow()
{
}

void MainWindow::addLevelClicked()
{
    qDebug() << table_levels->currentRow();
    if(table_levels->currentRow() >= 0) {
        table_levels->insertRow(table_levels->currentRow());
    }
    else {
        table_levels->setRowCount(table_levels->rowCount() + 1);
    }
}

void MainWindow::remLevelClicked()
{
    if(table_levels->rowCount() > 0) {
        if(table_levels->currentRow() < 0) {
            table_levels->setRowCount(table_levels->rowCount() - 1);
        }
        else {
            table_levels->removeRow(table_levels->currentRow());
        }
    }
}

void MainWindow::startClicked()
{'
    qDebug() << this->size();
    if(QMessageBox::question(this, "Inizia Torneo", "Dopo aver iniziato il torneo non potrai più modificare le impostazioni.\nContinuare?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
        return;

    Tournament *tournament = new Tournament(this);
    tournament->setName();

    TimerView *tv = new TimerView();
    tv->setAttribute(Qt::WA_DeleteOnClose);
    tv->setWindowState(Qt::WindowFullScreen);
    tv->show();
}
