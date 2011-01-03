#ifndef GLOBAL_H
#define GLOBAL_H

#define PTM_VERSION "0.1.1"
#define PTM_VERSION_MAJOR 0
#define PTM_VERSION_MINOR 1
#define PTM_VERSION_PATCH 1

#define PTM_XML_VERSION "0.1"
#define PTM_XML_VERSION_MAJOR 0
#define PTM_XML_VERSION_MINOR 1

#define PTM_XML_DOCTYPE "xml-pokertm"

#ifdef USE_QSOUND
#include <QSound>
#define PLAYSOUND(fileName) \
    QSound::play(fileName);
#else
#include <MediaObject>
#include <AudioOutput>
#include <MediaObject>
#include <Path>
#define PLAYSOUND(fileName) \
    Phonon::MediaObject * player = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource(fileName)); \
    QObject::connect(player, SIGNAL(finished()), player, SLOT(deleteLater())); \
    player->play();
#endif

#endif // GLOBAL_H
