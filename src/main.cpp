/***************************************************************************
 *                                                                         *
 *   This file is part of the PokerTournamentManager project,              *
 *       http://www.gitorious.org/poker-tournament-manager                 *
 *                                                                         *
 *   Authors: Alessandro Costa <kost.on.linux@gmail.com>, (C) 2009         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation. For more details see the   *
 *   COPYING.txt file that comes with this distribution.                   *
 *                                                                         *
 ***************************************************************************/

#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
#if QT_VERSION >= 0x040500
    // use the Raster GraphicsSystem as default on 4.5+
    QApplication::setGraphicsSystem("raster");
#endif

    QApplication a(argc, argv);
    a.setApplicationName("PokerTournamentManager");

    MainWindow w;
    w.show();

    return a.exec();
}
