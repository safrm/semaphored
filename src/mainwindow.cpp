/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://safrm.net/semaphored
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
    deleteAllAct(NULL),
    loadBackgroundImageAct(NULL),
    m_canvasWidget(new DragWidget()) //TODO use size hint in  canvas
{
    //createDockWindows();
    createActions();
    createMenus();
    //createToolBars();
    //createStatusBar();


    setCentralWidget(m_canvasWidget);
    //TODO  setWindowTitle("");
}
void MainWindow::createActions()
{
    exportAsPictureAct = new QAction(QIcon(":/images/export_as_picture.svg"), tr("&Export As picture"), this);
    exportAsPictureAct->setStatusTip(tr("Export As picture"));
    connect(exportAsPictureAct, SIGNAL(triggered()), this, SLOT(exportAsPictureSlot()));

    deleteAllAct = new QAction(QIcon(":/images/deleteall.svg"), tr("&Delete All"), this);
    deleteAllAct->setStatusTip(tr("Delete all"));
    connect(deleteAllAct, SIGNAL(triggered()), m_canvasWidget, SLOT(deleteAllItemsSlot()));

    loadBackgroundImageAct = new QAction(QIcon(":/images/load_background_image.svg"), tr("&Load background image"), this);
    loadBackgroundImageAct->setStatusTip(tr("Load background image"));
    connect(loadBackgroundImageAct, SIGNAL(triggered()), m_canvasWidget, SLOT(loadBackgroundImageSlot()));

    QActionGroup* backgroundColorGroup = new QActionGroup(this);
    m_BgColorWhiteAction = new QAction(QIcon(":/images/white.svg"), tr("&White"), this);
    m_BgColorWhiteAction->setCheckable(true);
    m_BgColorWhiteAction->setChecked(true);

    m_BgColorGrayAction = new QAction(QIcon(":/images/gray.svg"), tr("&Gray"), this);
    m_BgColorGrayAction->setCheckable(true);

    m_BgColorCyanAction = new QAction(QIcon(":/images/cyan.svg"), tr("&Cyan"), this);
    m_BgColorCyanAction->setCheckable(true);

    backgroundColorGroup->addAction(m_BgColorWhiteAction);
    backgroundColorGroup->addAction(m_BgColorGrayAction);
    backgroundColorGroup->addAction(m_BgColorCyanAction);
    backgroundColorGroup->setExclusive(true);
    connect(backgroundColorGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeBackgroundColorSlot(QAction*)));
}

void MainWindow::createMenus()
{
    QMenu* fileMenu =menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exportAsPictureAct);
    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(deleteAllAct);

    //menuBar()->addMenu(tr("&View"));
    QMenu* backgroundMenu = menuBar()->addMenu(tr("&Background"));
    backgroundMenu->addAction(m_BgColorWhiteAction);
    backgroundMenu->addAction(m_BgColorGrayAction);
    backgroundMenu->addAction(m_BgColorCyanAction);
    backgroundMenu->addSeparator();
    backgroundMenu->addAction(loadBackgroundImageAct);

    menuBar()->addSeparator();
    menuBar()->addMenu(tr("&Help"));
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

void MainWindow::changeBackgroundColorSlot(QAction * action)
{
    QColor newColor(Qt::white);
    if (action == m_BgColorWhiteAction )
        newColor = Qt::white;
    else if (action == m_BgColorGrayAction)
        newColor = Qt::gray;
    else if (action == m_BgColorCyanAction)
        newColor = Qt::cyan;

    m_canvasWidget->changeBackgroundColor(newColor);
}
