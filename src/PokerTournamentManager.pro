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

# Input files
HEADERS += MainWindow.h \
    TimerView.h \
    Tournament.h \
    AboutWidget.h

SOURCES += main.cpp \
    MainWindow.cpp \
    TimerView.cpp \
    Tournament.cpp \
    AboutWidget.cpp

FORMS += MainWindow.ui \
    AboutWidget.ui

RESOURCES += data.qrc

# windows
win32: DEFINES += USE_QSOUND
else: {
    QT += phonon
    INCLUDEPATH += /usr/include/phonon
}
