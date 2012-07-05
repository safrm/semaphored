#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QAction;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    
signals:
    
public slots:
    void deleteAllSlot();
private:
    void createActions();
    void createMenus();
//    void createToolBars();
//    void createStatusBar();
//    void createDockWindows();

    QAction* deleteAllAct;
};

#endif // MAINWINDOW_H
