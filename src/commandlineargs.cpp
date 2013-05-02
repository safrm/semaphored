/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://semaphored.safrm.net/
** Source: http://safrm.net/projects/semaphored
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
#include <QDebug>
#include <QStringList>
#include <QFileInfo>
#include <QProcess>
#include <stdio.h>

#include "commandlineargs.h"

CommandLineArgs* CommandLineArgs::instance = NULL;

CommandLineArgs::CommandLineArgs() :
    m_bMaximized(false),
    m_bMinimizedInTray(false),
    FilesToOpenList(),
    SwitchesArgs()
{
    QString arg("");
    QStringListIterator argsIterator(qApp->arguments());
    while (argsIterator.hasNext()) {
        //first argument is file executable
        if(!argsIterator.hasPrevious()) {
            argsIterator.next();
            continue;
        }
        arg = argsIterator.next();
        //options
        if (arg.startsWith("-")) {
            if (arg.startsWith("-m")) {
                m_bMaximized = false;
            } else if (arg.startsWith("-t")) {
                m_bMinimizedInTray = true;
            } else {
                qCritical() << "Unknown parameter:" << arg;
            }
            SwitchesArgs.append(arg);
        } else  { //files
            if(QFileInfo(arg).exists())
                FilesToOpenList.append(QFileInfo(arg).absoluteFilePath());
            else {
                qCritical() << "File " << arg << " does ";
                printUsageShort();
            }
       }
    }
}

CommandLineArgs* CommandLineArgs::getInstance()
{
    if(!instance)
        instance = new CommandLineArgs();
    return instance;
}

void CommandLineArgs::printUsage()
{
    //TODO
    printf("USAGE: %s: \n", QApplication::applicationName().toLatin1().data());
    printf("    parameters list                     \n");
    printf("-t  --tray      start minimized in tray\n");
    printf("-m  --maximized start with maximized window\n");
    printf("<files>         project files to load\n");
    qFatal(" ");
}

void CommandLineArgs::printUsageShort()
{
    //TODO
    printf("USAGE: %s --help or -? shows full help\n", QApplication::applicationName().toLatin1().data());
    qFatal(" ");
}
void CommandLineArgs::startNewInstance(const QString & sFileToOpen)
{
    QStringList argumentsForNewInstance = SwitchesArgs;
    argumentsForNewInstance.append(sFileToOpen);
    QProcess::startDetached(QApplication::applicationFilePath(), argumentsForNewInstance);
}
const QStringList& CommandLineArgs::filesToOpenList()
{
    return FilesToOpenList;
}
