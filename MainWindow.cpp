#include "MainWindow.h"

#include <QtGui>
#include <QtXml>

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

    QObject::connect(action_new, SIGNAL(triggered()), this, SLOT(newTriggered()));
    QObject::connect(action_open, SIGNAL(triggered()), this, SLOT(openTriggered()));
    QObject::connect(action_save, SIGNAL(triggered()), this, SLOT(saveTriggered()));
    QObject::connect(action_saveas, SIGNAL(triggered()), this, SLOT(saveAsTriggered()));
    QObject::connect(action_exit, SIGNAL(triggered()), this, SLOT(exitTriggered()));

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
    //qDebug() << m_table_levels->currentRow();
    if(m_table_levels->currentRow() >= 0 && m_table_levels->currentRow() < m_table_levels->rowCount() - 1) {
        m_table_levels->insertRow(m_table_levels->currentRow() + 1);
    }
    else {
        m_table_levels->setRowCount(m_table_levels->rowCount() + 1);
    }
}

void MainWindow::exitTriggered()
{
    this->close();
}

void MainWindow::newTriggered()
{
    if(QMessageBox::question(this, "Nuovo Torneo", "Vuoi salvare il torneo attuale prima di cancellarlo?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
        saveAsTriggered();
    }

    m_txt_name->clear();
    m_table_levels->clearContents();
    m_table_levels->setRowCount(0);
    m_sbox_chips->setValue(0);
    m_sbox_ngiocatori->setValue(0);
    m_sbox_rebuychips->setValue(0);
    m_sbox_rebuylev->setValue(0);
}

void MainWindow::openTriggered()
{
    newTriggered();

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
    m_txt_name->setText(node_tournament.attributes().namedItem("name").nodeValue());

    QDomNode node_levels = node_tournament.childNodes().at(0);
    for(int i = 0; i < node_levels.childNodes().size(); i++) {
        QDomNode node_lev = node_levels.childNodes().at(i);
        m_table_levels->setRowCount(i+1);

        m_table_levels->setItem(i, 0, new QTableWidgetItem(""));
        m_table_levels->setItem(i, 1, new QTableWidgetItem(""));
        m_table_levels->setItem(i, 2, new QTableWidgetItem(""));
        m_table_levels->setItem(i, 3, new QTableWidgetItem(""));

        m_table_levels->item(i, 0)->setText(node_lev.attributes().namedItem("time").nodeValue());
        m_table_levels->item(i, 1)->setText(node_lev.attributes().namedItem("ante").nodeValue());
        m_table_levels->item(i, 2)->setText(node_lev.attributes().namedItem("smallblind").nodeValue());
        m_table_levels->item(i, 3)->setText(node_lev.attributes().namedItem("bigblind").nodeValue());

        //qDebug() << node_lev.attributes().namedItem("time").nodeValue() << node_lev.attributes().namedItem("ante").nodeValue() << node_lev.attributes().namedItem("smallblind").nodeValue() << node_lev.attributes().namedItem("bigblind").nodeValue();
    }

    QDomNode node_players = node_tournament.childNodes().at(1);
    m_sbox_ngiocatori->setValue(QVariant(node_players.attributes().namedItem("number").nodeValue()).toInt());
    m_sbox_chips->setValue(QVariant(node_players.attributes().namedItem("chipseach").nodeValue()).toInt());

    QDomNode node_rebuy = node_tournament.childNodes().at(2);
    m_sbox_rebuylev->setValue(QVariant(node_rebuy.attributes().namedItem("maxlevel").nodeValue()).toInt());
    m_sbox_rebuychips->setValue(QVariant(node_rebuy.attributes().namedItem("chips").nodeValue()).toInt());

    //qDebug() << doc.toString();
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

void MainWindow::saveTriggered()
{
    saveAsTriggered();
}

void MainWindow::saveAsTriggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Salva come..."), QDir::homePath(), tr("Poker Tournament (*.pkt)"));
    if(fileName.isNull())
        return;

    QDomDocument doc("xml-pokertimer");
    QDomElement el_tournament = doc.createElement("tournament");
    el_tournament.setAttribute("name", m_txt_name->text());
    doc.appendChild(el_tournament);

    QDomElement el_levels = doc.createElement("levels");
    el_tournament.appendChild(el_levels);

    for(int i = 0; i < m_table_levels->rowCount(); i++) {
        QDomElement el_lev = doc.createElement("level");
        el_lev.setAttribute("time", m_table_levels->item(i,0)->text());
        el_lev.setAttribute("ante", m_table_levels->item(i,1)->text());
        el_lev.setAttribute("smallblind", m_table_levels->item(i,2)->text());
        el_lev.setAttribute("bigblind", m_table_levels->item(i,3)->text());
        el_levels.appendChild(el_lev);
    }

    QDomElement el_players = doc.createElement("players");
    el_players.setAttribute("number", QString::number(m_sbox_ngiocatori->value()));
    el_players.setAttribute("chipseach", QString::number(m_sbox_chips->value()));
    el_tournament.appendChild(el_players);

    QDomElement el_rebuy = doc.createElement("rebuy");
    el_rebuy.setAttribute("maxlevel", QString::number(m_sbox_rebuylev->value()));
    el_rebuy.setAttribute("chips", QString::number(m_sbox_rebuychips->value()));
    el_tournament.appendChild(el_rebuy);

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(doc.toByteArray());
    file.close();
}

void MainWindow::startClicked()
{
    //qDebug() << this->size();
    if(QMessageBox::question(this, "Inizia Torneo", "Dopo aver iniziato il torneo non potrai piu' modificare le impostazioni.\nContinuare?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
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

        //qDebug() << "Livello" << i << "-" << level.time_minutes << level.ante << level.smallblind << level.bigblind;
    }

    TimerView *tv = new TimerView(tournament);
    tv->setAttribute(Qt::WA_DeleteOnClose);
    tv->setWindowState(Qt::WindowFullScreen);
    tv->show();
}
