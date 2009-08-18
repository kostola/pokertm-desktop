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
    m_txt_name = new QLineEdit();
    QHBoxLayout *lay_name = new QHBoxLayout();
    lay_name->addWidget(m_txt_name);
    QGroupBox *gbox_name = wrapInGroupBox(lay_name, tr("Nome Torneo"));

    QStringList header_labels;
    header_labels << "Tempo" << "Ante" << "Piccolo Buio" << "Grande Buio";

    m_table_levels = new QTableWidget();
    m_table_levels->setMinimumWidth(500);
    m_table_levels->setColumnCount(4);
    m_table_levels->setRowCount(0);
    m_table_levels->setHorizontalHeaderLabels(header_labels);
    m_table_levels->setColumnWidth(0, 125);
    m_table_levels->setColumnWidth(1, 125);
    m_table_levels->setColumnWidth(2, 125);
    m_table_levels->setColumnWidth(3, 125);

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
    lay_levels->addWidget(m_table_levels);
    lay_levels->addLayout(lay_levels_button);

    QGroupBox *gbox_levels = wrapInGroupBox(lay_levels, tr("Gestione Livelli"));

    QLabel *lb1 = new QLabel("Giocatori:");
    QLabel *lb2 = new QLabel("Chips a testa:");

    m_sbox_ngiocatori = new QSpinBox();
    m_sbox_ngiocatori->setRange(0, 100000);
    m_sbox_ngiocatori->setMinimumWidth(160);
    m_sbox_chips = new QSpinBox();
    m_sbox_chips->setRange(0, 10000000);
    m_sbox_chips->setMinimumWidth(160);

    QHBoxLayout *lay_chips = new QHBoxLayout();
    lay_chips->addWidget(lb1, Qt::AlignRight);
    lay_chips->addWidget(m_sbox_ngiocatori);
    lay_chips->addStretch(5);
    lay_chips->addWidget(lb2, Qt::AlignRight);
    lay_chips->addWidget(m_sbox_chips);

    QGroupBox *gbox_chips = wrapInGroupBox(lay_chips, tr("Gestione Chips"));

    QLabel *lb3 = new QLabel("Livello:");
    QLabel *lb4 = new QLabel("Chips:");

    m_sbox_rebuylev = new QSpinBox();
    m_sbox_rebuylev->setRange(0, 100000);
    m_sbox_rebuylev->setMinimumWidth(160);
    m_sbox_rebuychips = new QSpinBox();
    m_sbox_rebuychips->setRange(0, 10000000);
    m_sbox_rebuychips->setMinimumWidth(160);

    QHBoxLayout *lay_rebuy = new QHBoxLayout();
    lay_rebuy->addWidget(lb3, Qt::AlignRight);
    lay_rebuy->addWidget(m_sbox_rebuylev);
    lay_rebuy->addStretch(5);
    lay_rebuy->addWidget(lb4, Qt::AlignRight);
    lay_rebuy->addWidget(m_sbox_rebuychips);

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
    qDebug() << m_table_levels->currentRow();
    if(m_table_levels->currentRow() >= 0) {
        m_table_levels->insertRow(m_table_levels->currentRow());
    }
    else {
        m_table_levels->setRowCount(m_table_levels->rowCount() + 1);
    }
}

void MainWindow::remLevelClicked()
{
    if(m_table_levels->rowCount() > 0) {
        if(m_table_levels->currentRow() < 0) {
            m_table_levels->setRowCount(m_table_levels->rowCount() - 1);
        }
        else {
            m_table_levels->removeRow(m_table_levels->currentRow());
        }
    }
}

void MainWindow::startClicked()
{
    qDebug() << this->size();
    if(QMessageBox::question(this, "Inizia Torneo", "Dopo aver iniziato il torneo non potrai più modificare le impostazioni.\nContinuare?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
        return;

    Tournament *tournament = new Tournament(this);
    tournament->setName(m_txt_name->text());
    tournament->setCurrentPlayers(m_sbox_ngiocatori->value());
    tournament->setTotalPlayers(m_sbox_ngiocatori->value());
    tournament->setChipsEach(m_sbox_chips->value());
    tournament->setRebuyMaxLevel(m_sbox_rebuylev->value());
    tournament->setRebuyChips(m_sbox_rebuychips->value());

    for(int i = 0; i < m_table_levels->rowCount(); i++) {
        Tournament::Level level;
        level.time_minutes = QVariant(m_table_levels->item(i,0)->text()).toInt();
        level.ante = QVariant(m_table_levels->item(i,1)->text()).toInt();
        level.smallblind = QVariant(m_table_levels->item(i,2)->text()).toInt();
        level.bigblind = QVariant(m_table_levels->item(i,3)->text()).toInt();
        tournament->addLevel(level);

        qDebug() << "Livello" << i << "-" << level.time_minutes << level.ante << level.smallblind << level.bigblind;
    }

    TimerView *tv = new TimerView(tournament);
    tv->setAttribute(Qt::WA_DeleteOnClose);
    tv->setWindowState(Qt::WindowFullScreen);
    tv->show();
}
