#ifndef COMMANDLINEARGS_H
#define COMMANDLINEARGS_H
class QStringList;
class QString;
class CommandLineArgs
{
public:
    explicit CommandLineArgs();
    static CommandLineArgs* getInstance();
    static void printUsage();
    static void printUsageShort();
    void startNewInstance(const QString & sFileToOpen);
    const QStringList& filesToOpenList();

private:
    static CommandLineArgs* instance;
    QStringList FilesToOpenList;
    QStringList SwitchesArgs;
};

#endif // COMMANDLINEARGS_H
