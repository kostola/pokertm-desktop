# -------------------------------------------------
# Project created by QtCreator 2009-08-14T15:10:19
# -------------------------------------------------
TARGET = PokerTournamentManager
QT += xml \
    phonon
TEMPLATE = app
HEADERS += MainWindow.h \
    TimerView.h \
    Tournament.h
SOURCES += main.cpp \
    MainWindow.cpp \
    TimerView.cpp \
    Tournament.cpp
FORMS += MainWindow.ui
RESOURCES += data.qrc
OBJECTS_DIR = .obj
MOC_DIR = .obj
RCC_DIR = .obj
UI_DIR = .obj
INCLUDEPATH += /usr/include/KDE
