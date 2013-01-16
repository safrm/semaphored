#include <QApplication>
#include <QDebug>
#include <QStringList>
#include <QFileInfo>
#include <stdio.h>

#include "commandlineargs.h"

CommandLineArgs* CommandLineArgs::instance = NULL;

CommandLineArgs::CommandLineArgs() :
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
                ;//TODO run minimized
            } else if (arg.startsWith("-t")) {
                ;//TODO tray
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
            //cout << javaStyleIterator.next().toLocal8Bit().constData()
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
    qFatal("");
}

void CommandLineArgs::printUsageShort()
{
    //TODO
    printf("USAGE: %s --help or -? shows full help\n", QApplication::applicationName().toLatin1().data());
    qFatal("");
}
const QStringList& CommandLineArgs::filesToOpenList()
{
    return FilesToOpenList;
}
const QStringList& CommandLineArgs::switchesArgs()
{
    return SwitchesArgs;
}
