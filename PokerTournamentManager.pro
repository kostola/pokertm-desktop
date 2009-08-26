# -------------------------------------------------
# Project created by QtCreator 2009-08-14T15:10:19
# -------------------------------------------------
TEMPLATE = app
TARGET = PokerTournamentManager
QT = core gui xml
OBJECTS_DIR = .obj
MOC_DIR = .obj
RCC_DIR = .obj
UI_DIR = .obj
INCLUDEPATH += /usr/include/KDE

# Input files
HEADERS += MainWindow.h \
    TimerView.h \
    Tournament.h

SOURCES += main.cpp \
    MainWindow.cpp \
    TimerView.cpp \
    Tournament.cpp

FORMS += MainWindow.ui

RESOURCES += data.qrc

# windows
win32 {
    contains(CONFIG, static) {
        DEFINES += STATIC_LINK
        QTPLUGIN += qjpeg
    }
} else {
    QT += phonon
}
