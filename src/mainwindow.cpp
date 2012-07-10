#include <QMenuBar>
#include <QFileDialog>
#include <QImageWriter>
#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"
#include "draglabel.h"
#include "dragwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    exportAsPictureAct(NULL),
    deleteAllAct(NULL)
{

    //createDockWindows();
    createActions();
    createMenus();
    //createToolBars();
    //createStatusBar();


    DragWidget* window = new DragWidget();
    setCentralWidget(window);
    //TODO  setWindowTitle("");
}
void MainWindow::createActions()
{
    exportAsPictureAct = new QAction(QIcon(":/images/export_as_picture.svg"), tr("&Export As picture"), this);
    exportAsPictureAct->setStatusTip(tr("Export As picture"));
    connect(exportAsPictureAct, SIGNAL(triggered()), this, SLOT(exportAsPictureSlot()));

    deleteAllAct = new QAction(QIcon(":/images/deleteall.svg"), tr("&Delete All"), this);
    deleteAllAct->setStatusTip(tr("Delete all"));
    connect(deleteAllAct, SIGNAL(triggered()), this, SLOT(deleteAllSlot()));
}

void MainWindow::createMenus()
{
    QMenu* fileMenu =menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exportAsPictureAct);
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

void MainWindow::exportAsPictureSlot()
{
    QString supportedFormats("");
    foreach(QByteArray name,QImageWriter::supportedImageFormats())
        supportedFormats +=  name + " ";
    QString sFilename = QFileDialog::getSaveFileName(this, "Save as picture as: " + supportedFormats,"untitled.png",
                                                     tr("Images (*.png *.xpm *.jpg)"));
    if(sFilename.size()) {
        QByteArray ext = QFileInfo(sFilename).suffix().toLower().toLatin1();
        //suffix = suffix.mid(suffix.lastIndexOf('.')); - grabs the last period in addition to the suffix
        if(QImageWriter::supportedImageFormats().contains(ext)) {
            QPixmap pix = QPixmap::grabWidget(centralWidget());
            pix.save(sFilename, ext);
        } else {
            QMessageBox::warning(this,"Picture was not exported", "Your system supports only following formats: " +supportedFormats);
        }
    }
}
