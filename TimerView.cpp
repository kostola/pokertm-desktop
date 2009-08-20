#include "TimerView.h"

#include <QtGui>
#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>
#include <Phonon/Path>

#define FONT_NAME "Helvsetica"
#define MARGIN 5.0
#define SCREEN_WIDTH  1276.0
#define SCREEN_HEIGHT 796.0
#define SIDEBANDS_WIDTH 300.0
#define TITLE_FONT QFont(FONT_NAME, 28)

QString addNumberPoints(const QString& n)
{
    QString num;
    int counter = 0;
    for(int i = n.size() - 1; i >= 0; i--) {
        if(counter > 0 && counter % 3 == 0)
            num.prepend(".");
        num.prepend(n.mid(i,1));
        counter++;
    }

    return num;
}

qreal calcHCenter(QGraphicsItem *item, qreal lbound, qreal rbound)
{
    return lbound + (rbound - lbound - item->boundingRect().width()) / 2.0;
}

qreal calcVCenter(QGraphicsItem *item, qreal ubound, qreal lbound)
{
    return ubound + (lbound - ubound - item->boundingRect().height()) / 2.0;
}

void setFontSizeAndCenter(QGraphicsTextItem *item, const QRectF &rect)
{
    QFont font = item->font();
    for(int i = 400; i >= 15; i -= 5) {
        font.setPixelSize(i);
        item->setFont(font);
        if(item->boundingRect().width() <= rect.width() && item->boundingRect().height() <= rect.height())
            break;
    }

    item->setPos(calcHCenter(item, rect.left(), rect.right()), calcVCenter(item, rect.top(), rect.bottom()));
}

TimerView::TimerView(Tournament *t)
    : m_tournament(t)
    , m_paused(true)
    , m_current_level(0)
{
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setBackgroundBrush(QBrush(Qt::black));

    QPen pen_white(QBrush(Qt::white), 3.0);
    /*QGraphicsLineItem *l_v1 = */scene->addLine(SIDEBANDS_WIDTH, 0.0, SIDEBANDS_WIDTH, SCREEN_HEIGHT, pen_white);
    /*QGraphicsLineItem *l_v2 = */scene->addLine(SCREEN_WIDTH - SIDEBANDS_WIDTH, 0.0, SCREEN_WIDTH - SIDEBANDS_WIDTH, SCREEN_HEIGHT, pen_white);
    /*QGraphicsLineItem *l_h1 = */scene->addLine(0.0, SCREEN_HEIGHT / 3.0, SIDEBANDS_WIDTH, SCREEN_HEIGHT / 3.0, pen_white);
    /*QGraphicsLineItem *l_h2 = */scene->addLine(0.0, SCREEN_HEIGHT / 3.0 * 2.0, SIDEBANDS_WIDTH, SCREEN_HEIGHT / 3.0 * 2.0, pen_white);
    /*QGraphicsLineItem *l_h3 = */scene->addLine(SCREEN_WIDTH - SIDEBANDS_WIDTH, SCREEN_HEIGHT / 3.0, SCREEN_WIDTH, SCREEN_HEIGHT / 3.0, pen_white);
    /*QGraphicsLineItem *l_h4 = */scene->addLine(SCREEN_WIDTH - SIDEBANDS_WIDTH, SCREEN_HEIGHT / 3.0 * 2.0, SCREEN_WIDTH, SCREEN_HEIGHT / 3.0 * 2.0, pen_white);

    QGraphicsTextItem *txt_1 = scene->addText(tr("Giocatori"), TITLE_FONT);
    txt_1->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_2 = scene->addText(tr("Avg. Stack"), TITLE_FONT);
    txt_2->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_3 = scene->addText(tr("Total Chips"), TITLE_FONT);
    txt_3->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_4 = scene->addText(tr("Inizio"), TITLE_FONT);
    txt_4->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_5 = scene->addText(tr("Tempo di gioco"), TITLE_FONT);
    txt_5->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_6 = scene->addText(tr("Ora"), TITLE_FONT);
    txt_6->setDefaultTextColor(Qt::white);

    txt_1->setPos(calcHCenter(txt_1, 0.0, SIDEBANDS_WIDTH), 0.0);
    txt_2->setPos(calcHCenter(txt_2, 0.0, SIDEBANDS_WIDTH), SCREEN_HEIGHT / 3.0);
    txt_3->setPos(calcHCenter(txt_3, 0.0, SIDEBANDS_WIDTH), SCREEN_HEIGHT / 3.0 * 2.0);
    txt_4->setPos(calcHCenter(txt_4, SCREEN_WIDTH - SIDEBANDS_WIDTH, SCREEN_WIDTH), 0.0);
    txt_5->setPos(calcHCenter(txt_5, SCREEN_WIDTH - SIDEBANDS_WIDTH, SCREEN_WIDTH), SCREEN_HEIGHT / 3.0);
    txt_6->setPos(calcHCenter(txt_6, SCREEN_WIDTH - SIDEBANDS_WIDTH, SCREEN_WIDTH), SCREEN_HEIGHT / 3.0 * 2.0);

    qreal sidebands_box_w = SIDEBANDS_WIDTH - 2.0 * MARGIN;
    qreal sidebands_box_h = SCREEN_HEIGHT / 3.0 - txt_1->boundingRect().height() - 2.0 * MARGIN;

    box_1 = QRectF(MARGIN, txt_1->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    box_2 = QRectF(MARGIN, SCREEN_HEIGHT / 3.0 + txt_2->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    box_3 = QRectF(MARGIN, SCREEN_HEIGHT / 3.0 * 2.0 + txt_2->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    box_4 = QRectF(SCREEN_WIDTH - SIDEBANDS_WIDTH + MARGIN, txt_1->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    box_5 = QRectF(SCREEN_WIDTH - SIDEBANDS_WIDTH + MARGIN, SCREEN_HEIGHT / 3.0 + txt_2->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    box_6 = QRectF(SCREEN_WIDTH - SIDEBANDS_WIDTH + MARGIN, SCREEN_HEIGHT / 3.0 * 2.0 + txt_2->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);

    m_txt_players = scene->addText("", QFont(FONT_NAME));
    m_txt_players->setDefaultTextColor(Qt::white);
    updatePlayers();

    m_txt_avgstack = scene->addText("", QFont(FONT_NAME));
    m_txt_avgstack->setDefaultTextColor(Qt::white);
    updateAverageStack();

    m_txt_totchips = scene->addText("", QFont(FONT_NAME));
    m_txt_totchips->setDefaultTextColor(Qt::white);
    updateTotalChips();

    m_txt_starttime = scene->addText("00:00:00", QFont(FONT_NAME));
    m_txt_starttime->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(m_txt_starttime, box_4);

    m_txt_playtime = scene->addText("00:00:00", QFont(FONT_NAME));
    m_txt_playtime->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(m_txt_playtime, box_5);

    m_txt_time = scene->addText(QTime::currentTime().toString("HH:mm:ss"), QFont(FONT_NAME));
    m_txt_time->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(m_txt_time, box_6);

    int button_size = (int) ((SCREEN_WIDTH - 2.0 * SIDEBANDS_WIDTH - 3.0 * MARGIN) / 7.0 - MARGIN);

    QGraphicsLinearLayout *lay_buttons = new QGraphicsLinearLayout(Qt::Horizontal);

    QPushButton *pb_play = new QPushButton("Play");
    QPalette pal = pb_play->palette();
    pal.setColor(QPalette::Window, Qt::darkGray);
    pal.setColor(QPalette::Button, QColor(0x20,0x20,0x20));
    pal.setColor(QPalette::ButtonText, Qt::white);
    pb_play->setPalette(pal);
    pb_play->setFlat(true);
    pb_play->setStyleSheet("border: 0px;");
    pb_play->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_play_item = scene->addWidget(pb_play);
    lay_buttons->addItem(pb_play_item);

    QPushButton *pb_prev = new QPushButton("Prev");
    pb_prev->setPalette(pal);
    pb_prev->setFlat(true);
    pb_prev->setStyleSheet("border: 0px;");
    pb_prev->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_prev_item = scene->addWidget(pb_prev);
    lay_buttons->addItem(pb_prev_item);

    QPushButton *pb_next = new QPushButton("Next");
    pb_next->setPalette(pal);
    pb_next->setFlat(true);
    pb_next->setStyleSheet("border: 0px;");
    pb_next->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_next_item = scene->addWidget(pb_next);
    lay_buttons->addItem(pb_next_item);

    QPushButton *pb_playerout = new QPushButton("Pl. Out");
    pb_playerout->setPalette(pal);
    pb_playerout->setFlat(true);
    pb_playerout->setStyleSheet("border: 0px;");
    pb_playerout->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_playerout_item = scene->addWidget(pb_playerout);
    lay_buttons->addItem(pb_playerout_item);

    QPushButton *pb_rebuy = new QPushButton("Rebuy");
    pb_rebuy->setPalette(pal);
    pb_rebuy->setFlat(true);
    pb_rebuy->setStyleSheet("border: 0px;");
    pb_rebuy->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_rebuy_item = scene->addWidget(pb_rebuy);
    lay_buttons->addItem(pb_rebuy_item);

    QPushButton *pb_minimize = new QPushButton("Minimize");
    pb_minimize->setPalette(pal);
    pb_minimize->setFlat(true);
    pb_minimize->setStyleSheet("border: 0px;");
    pb_minimize->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_minimize_item = scene->addWidget(pb_minimize);
    lay_buttons->addItem(pb_minimize_item);

    QPushButton *pb_close = new QPushButton("Close");
    pb_close->setPalette(pal);
    pb_close->setFlat(true);
    pb_close->setStyleSheet("border: 0px;");
    pb_close->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_close_item = scene->addWidget(pb_close);
    lay_buttons->addItem(pb_close_item);

    QGraphicsWidget *buttons_item = new QGraphicsWidget();
    buttons_item->setLayout(lay_buttons);

    //qDebug() << buttons_item->boundingRect();

    scene->addItem(buttons_item);
    buttons_item->setPos(SIDEBANDS_WIDTH, SCREEN_HEIGHT - button_size - 3.0 * MARGIN);

    QObject::connect(pb_play, SIGNAL(clicked()), this, SLOT(playClicked()));
    QObject::connect(pb_prev, SIGNAL(clicked()), this, SLOT(prevClicked()));
    QObject::connect(pb_next, SIGNAL(clicked()), this, SLOT(nextClicked()));
    QObject::connect(pb_playerout, SIGNAL(clicked()), this, SLOT(playerOutClicked()));
    QObject::connect(pb_rebuy, SIGNAL(clicked()), this, SLOT(rebuyClicked()));
    QObject::connect(pb_minimize, SIGNAL(clicked()), this, SLOT(showMinimized()));
    QObject::connect(pb_close, SIGNAL(clicked()), this, SLOT(close()));

    box_name        = QRectF(SIDEBANDS_WIDTH + MARGIN, 0.0, SCREEN_WIDTH - 2.0 * (SIDEBANDS_WIDTH + MARGIN), 85.0);
    box_timer       = QRectF(SIDEBANDS_WIDTH + MARGIN, box_name.bottom() * 0.8, SCREEN_WIDTH - 2.0 * (SIDEBANDS_WIDTH + MARGIN), SCREEN_HEIGHT / 3.0);
    box_blinds      = QRectF(SIDEBANDS_WIDTH + MARGIN, box_timer.bottom() * 0.9, SCREEN_WIDTH - 2.0 * (SIDEBANDS_WIDTH + MARGIN), (SCREEN_HEIGHT - button_size - 3.0 * MARGIN - box_timer.bottom() * 0.9) / 9.0 * 4.0);
    box_ante        = QRectF(SIDEBANDS_WIDTH + MARGIN, box_blinds.top() + box_blinds.height() * 0.85, SCREEN_WIDTH - 2.0 * (SIDEBANDS_WIDTH + MARGIN), (SCREEN_HEIGHT - button_size - 3.0 * MARGIN - box_timer.bottom() * 0.9) / 9.0 * 2.5);
    box_next_blinds = QRectF(SIDEBANDS_WIDTH + MARGIN, box_ante.bottom(), SCREEN_WIDTH - 2.0 * (SIDEBANDS_WIDTH + MARGIN), (SCREEN_HEIGHT - button_size - 3.0 * MARGIN - box_timer.bottom() * 0.9) / 9.0 * 2.0);
    box_next_ante   = QRectF(SIDEBANDS_WIDTH + MARGIN, box_next_blinds.top() + box_next_blinds.height() * 0.85, SCREEN_WIDTH - 2.0 * (SIDEBANDS_WIDTH + MARGIN), (SCREEN_HEIGHT - button_size - 3.0 * MARGIN - box_timer.bottom() * 0.9) / 9.0 * 1.5);

    m_txt_name = scene->addText(m_tournament->getName(), QFont(FONT_NAME));
    m_txt_name->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(m_txt_name, box_name);

    m_txt_timer = scene->addText("88:88", QFont(FONT_NAME));
    m_txt_timer->setDefaultTextColor(Qt::red);
    setFontSizeAndCenter(m_txt_timer, box_timer);

    m_level_time = QTime(0, m_tournament->getLevel(m_current_level).time_minutes, 0, 0);
    m_play_time = QTime(0,0,0,0);
    m_txt_timer->setPlainText(m_level_time.toString("mm:ss"));

    m_txt_blinds = scene->addText("", QFont(FONT_NAME));
    m_txt_blinds->setDefaultTextColor(Qt::white);

    m_txt_ante = scene->addText("", QFont(FONT_NAME));
    m_txt_ante->setDefaultTextColor(Qt::white);

    m_txt_next_blinds = scene->addText("400 / 800", QFont(FONT_NAME));
    m_txt_next_blinds->setDefaultTextColor(Qt::white);

    m_txt_next_ante = scene->addText("Ante: 200", QFont(FONT_NAME));
    m_txt_next_ante->setDefaultTextColor(Qt::white);

    updateLevels();

    this->setScene(scene);
    this->setSceneRect(0.0, 0.0, SCREEN_WIDTH, SCREEN_HEIGHT);
    this->setWindowTitle("PokerTimer - Torneo");

    QTimer *time_timer = new QTimer(this);
    QObject::connect(time_timer, SIGNAL(timeout()), this, SLOT(updateCurrentTime()));
    time_timer->start(1000);

    m_tournament_timer = new QTimer(this);
    QObject::connect(m_tournament_timer, SIGNAL(timeout()), this, SLOT(tournamentTimerTimeout()));
}

void TimerView::nextClicked()
{
    m_current_level++;
    updateLevels();
    m_level_time = QTime(0, m_tournament->getLevel(m_current_level).time_minutes, 0, 0);
    updatePlayTimes();
    if(! m_paused)
        m_tournament_timer->start(1000);
}

void TimerView::playClicked()
{
    if(m_paused) {
        //qDebug() << "PLAY CLICKED PAUSED";
        if(m_txt_starttime->toPlainText() == "00:00:00" && m_play_time.toString("HH:mm:ss") == "00:00:00") {
            m_txt_starttime->setPlainText(QTime::currentTime().toString("HH:mm:ss"));
        }

        m_txt_timer->setDefaultTextColor(Qt::white);
        m_tournament_timer->start(1000);
        m_paused = false;
    }
    else {
        //qDebug() << "PLAY CLICKED NOT PAUSED";
        m_paused = true;
        m_tournament_timer->stop();
        m_txt_timer->setDefaultTextColor(Qt::red);
        m_txt_timer->update(m_txt_timer->boundingRect());
    }
}

void TimerView::playerOutClicked()
{
    m_tournament->playerOut();
    updatePlayers();
    updateAverageStack();
    updateTotalChips();
}

void TimerView::prevClicked()
{
    if(m_current_level == 0)
        return;

    m_current_level--;
    updateLevels();
    m_level_time = QTime(0, m_tournament->getLevel(m_current_level).time_minutes, 0, 0);
    updatePlayTimes();
    if(! m_paused)
        m_tournament_timer->start(1000);
}

void TimerView::rebuyClicked()
{
    m_tournament->rebuy(m_current_level);
    updatePlayers();
    updateAverageStack();
    updateTotalChips();
}

void TimerView::tournamentTimerTimeout()
{
    m_level_time = m_level_time.addSecs(-1);

    if(m_level_time.toString("mm:ss") == "00:00") {
        // cambio livello
        m_current_level++;
        updateLevels();
        m_level_time = QTime(0, m_tournament->getLevel(m_current_level).time_minutes, 0, 0);
        qDebug() << "PLAY GONG";
        m_sound_gong = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource("data/gong.wav"));
        QObject::connect(m_sound_gong, SIGNAL(finished()), m_sound_gong, SLOT(deleteLater()));
        m_sound_gong->play();
    }
    else if(m_level_time.toString("mm:ss") == "01:00") {
        qDebug() << "PLAY SCREAM";
        m_sound_scream = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource("data/scream.wav"));
        QObject::connect(m_sound_scream, SIGNAL(finished()), m_sound_scream, SLOT(deleteLater()));
        m_sound_scream->play();
    }
    else if(m_level_time.toString("mm:ss") == "00:01" || m_level_time.toString("mm:ss") == "00:02" || m_level_time.toString("mm:ss") == "00:03") {
        qDebug() << "PLAY TICK";
        m_sound_tick = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource("data/tick.wav"));
        QObject::connect(m_sound_tick, SIGNAL(finished()), m_sound_tick, SLOT(deleteLater()));
        m_sound_tick->play();
    }

    m_play_time = m_play_time.addSecs(1);
    updatePlayTimes();
    //qDebug() << m_level_time << m_play_time;
}

void TimerView::updateAverageStack()
{
    m_txt_avgstack->setPlainText(addNumberPoints(QString::number(m_tournament->getAverageStack())));
    setFontSizeAndCenter(m_txt_avgstack, box_2);
}

void TimerView::updateCurrentTime()
{
    m_txt_time->setPlainText(QTime::currentTime().toString("HH:mm:ss"));
}

void TimerView::updateLevels()
{
    m_txt_blinds->setPlainText(QString("%1 / %2").arg(m_tournament->getLevel(m_current_level).smallblind).arg(m_tournament->getLevel(m_current_level).bigblind));
    setFontSizeAndCenter(m_txt_blinds, box_blinds);

    m_txt_ante->setPlainText(QString("Ante: %1").arg(m_tournament->getLevel(m_current_level).ante));
    setFontSizeAndCenter(m_txt_ante, box_ante);

    m_txt_next_blinds->setPlainText(QString("%1 / %2").arg(m_tournament->getLevel(m_current_level + 1).smallblind).arg(m_tournament->getLevel(m_current_level + 1).bigblind));
    setFontSizeAndCenter(m_txt_next_blinds, box_next_blinds);

    m_txt_next_ante->setPlainText(QString("Ante: %1").arg(m_tournament->getLevel(m_current_level + 1).ante));
    setFontSizeAndCenter(m_txt_next_ante, box_next_ante);
}

void TimerView::updatePlayers()
{
    m_txt_players->setPlainText(QString("%1/%2").arg(m_tournament->getCurrentPlayers()).arg(m_tournament->getTotalPlayers()));
    setFontSizeAndCenter(m_txt_players, box_1);
}

void TimerView::updatePlayTimes()
{
    m_txt_timer->setPlainText(m_level_time.toString("mm:ss"));
    m_txt_playtime->setPlainText(m_play_time.toString("HH:mm:ss"));
}

void TimerView::updateTotalChips()
{
    m_txt_totchips->setPlainText(addNumberPoints(QString::number(m_tournament->getTotalChips())));
    setFontSizeAndCenter(m_txt_totchips, box_3);
}
