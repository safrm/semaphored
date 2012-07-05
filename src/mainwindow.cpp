#include <QMenuBar>
#include "mainwindow.h"
#include "dragwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    //createDockWindows();
    //createActions();
    createMenus();
    //createToolBars();
    //createStatusBar();


    DragWidget* window = new DragWidget();
    setCentralWidget(window);

}
void MainWindow::createMenus()
{
    menuBar()->addMenu(tr("&File"));
    menuBar()->addMenu(tr("&Edit"));
    menuBar()->addMenu(tr("&View"));
    menuBar()->addSeparator();
    menuBar()->addMenu(tr("&Help"));
    //editMenu->addAction(undoAct);
}
