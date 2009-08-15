#include "MainWindow.h"

#include <QtGui>

#include "TimerView.h"

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
    QTableWidget *table_levels = new QTableWidget();
    table_levels->setMinimumWidth(500);

    QPushButton *pb_add_level = new QPushButton("+");
    pb_add_level->setFont(QFont("Helvetica", 16, QFont::Bold));
    pb_add_level->setFixedSize(30,30);
    QPushButton *pb_rem_level = new QPushButton("-");
    pb_rem_level->setFont(QFont("Helvetica", 16, QFont::Bold));
    pb_rem_level->setFixedSize(30,30);

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

    QPushButton *pb_start = new QPushButton(tr("S  T  A  R  T"));
    pb_start->setFont(QFont("Helvetica", 18, QFont::Normal));

    QObject::connect(pb_start, SIGNAL(clicked()), this, SLOT(startClicked()));

    QVBoxLayout *lay_central = new QVBoxLayout();
    lay_central->addWidget(gbox_levels);
    lay_central->addWidget(gbox_chips);
    lay_central->addWidget(pb_start);

    QWidget *w_central = new QWidget();
    w_central->setLayout(lay_central);

    this->setCentralWidget(w_central);
    this->setMinimumSize(650, 520);
}

MainWindow::~MainWindow()
{
}


void MainWindow::startClicked()
{
    qDebug() << this->size();
    TimerView *tv = new TimerView();
    tv->setWindowState(Qt::WindowFullScreen);
    tv->show();
}
