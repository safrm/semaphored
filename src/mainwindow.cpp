#include <QMenuBar>
#include "mainwindow.h"
#include "draglabel.h"
#include "dragwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    deleteAllAct(NULL)
{

    //createDockWindows();
    createActions();
    createMenus();
    //createToolBars();
    //createStatusBar();


    DragWidget* window = new DragWidget();
    setCentralWidget(window);

}
void MainWindow::createActions()
{
    deleteAllAct = new QAction(QIcon(":/images/deleteall.svg"), tr("&Delete All"), this);
    deleteAllAct->setStatusTip(tr("Delete all"));
    connect(deleteAllAct, SIGNAL(triggered()), this, SLOT(deleteAllSlot()));

}

void MainWindow::createMenus()
{
    menuBar()->addMenu(tr("&File"));
    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(deleteAllAct);

    menuBar()->addMenu(tr("&View"));
    menuBar()->addSeparator();
    menuBar()->addMenu(tr("&Help"));
}

void MainWindow::deleteAllSlot()
{
    foreach (QObject *child, centralWidget()->children()) {
        if (child->inherits("DragLabel")) {
            DragLabel *widget = static_cast<DragLabel *>(child);
            //if (!widget->isVisible())
               widget->deleteLater();
        }
    }
}
