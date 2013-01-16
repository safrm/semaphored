#ifndef COMMANDLINEARGS_H
#define COMMANDLINEARGS_H
class QStringList;
class CommandLineArgs
{
public:
    explicit CommandLineArgs();
    static CommandLineArgs* getInstance();
    static void printUsage();
    static void printUsageShort();
    const QStringList& filesToOpenList();
    const QStringList& switchesArgs();

private:
    static CommandLineArgs* instance;
    QStringList FilesToOpenList;
    QStringList SwitchesArgs;
};

#endif // COMMANDLINEARGS_H
