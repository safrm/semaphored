/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://semaphored.safrm.net/
** Source: https://github.com/safrm/semaphored
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/

#include <QApplication>
#include <QSplashScreen>
#include "mainwindow.h"
#include "commandlineargs.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(semaphored);

    QApplication app(argc, argv);

    QString sProjectFullFileName("");
    foreach (const QString &FileToOpen, CommandLineArgs::getInstance()->filesToOpenList()) {
        if (sProjectFullFileName.isEmpty())
            sProjectFullFileName = FileToOpen;
        else { //recursive instance creation
            CommandLineArgs::getInstance()->startNewInstance(FileToOpen);
        }
    }
    //otherwise load default project
    if (sProjectFullFileName.isEmpty())
        sProjectFullFileName = ":/demo-projects/kanban1.sem";

    app.setAttribute(Qt::AA_DontShowIconsInMenus,false);
    QPixmap pixmap(":/images/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();

    MainWindow* mainWin = new MainWindow();
    mainWin->loadProject(sProjectFullFileName);
    if (!CommandLineArgs::getInstance()->m_bMinimizedInTray) {
        if (!CommandLineArgs::getInstance()->m_bMaximized)
            mainWin->showMaximized();
        else
            mainWin->show();
    }
#ifdef Q_OS_UNIX
    usleep(700);
#endif //Q_OS_UNIX
    splash.finish(mainWin);
    return app.exec();
}
