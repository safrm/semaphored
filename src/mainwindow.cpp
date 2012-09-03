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
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include "mainwindow.h"
#include "draglabel.h"
#include "dragwidget.h"

MainWindow * g_pMainGuiWindow =NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    loadProjectAct(NULL),
    saveProjectAct(NULL),
    exportAsPictureAct(NULL),
    loadTextFileAct(NULL),
    quitAct(NULL),
    deleteAllAct(NULL),
    m_canvasWidget(new DragWidget()) //TODO use size hint in  canvas
{
    g_pMainGuiWindow = this;
    //createDockWindows();
    createActions();
    createMenus();
    //createToolBars();
    //createStatusBar();


    setCentralWidget(m_canvasWidget);
    setWindowTitle(QString(APPLICATION_NAME) + "-" + QString(APP_VERSION_FULL) + " (" + QString(APP_DATE) +")");
}
MainWindow* MainWindow::instance()
{
    return  g_pMainGuiWindow;
}

void MainWindow::createActions()
{
    loadProjectAct = new QAction(QIcon(":/images/load_project.svg"), tr("&Load project"), this);
    loadProjectAct->setStatusTip(tr("Load project"));
    connect(loadProjectAct, SIGNAL(triggered()), this, SLOT(loadProjectSlot()));

    saveProjectAct = new QAction(QIcon(":/images/save_project.svg"), tr("&Save project"), this);
    saveProjectAct->setStatusTip(tr("Save project"));
    connect(saveProjectAct, SIGNAL(triggered()), this, SLOT(saveProjectSlot()));

    exportAsPictureAct = new QAction(QIcon(":/images/export_as_picture.svg"), tr("&Export As picture"), this);
    exportAsPictureAct->setStatusTip(tr("Export As picture"));
    connect(exportAsPictureAct, SIGNAL(triggered()), this, SLOT(exportAsPictureSlot()));

    loadTextFileAct = new QAction(QIcon(":/images/load_text_file.svg"), tr("&Load text file"), this);
    loadTextFileAct->setStatusTip(tr("Load text file"));
    connect(loadTextFileAct, SIGNAL(triggered()), this, SLOT(loadTextFileSlot()));

    printAct = new QAction(QIcon(":/images/print.svg"), tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the current canvas"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(printCurrentCanvas()));


    quitAct = new QAction(QIcon(":/images/quit.svg"), tr("&Quit"), this);
    quitAct->setStatusTip(tr("Quit"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    deleteAllAct = new QAction(QIcon(":/images/deleteall.svg"), tr("&Delete All"), this);
    deleteAllAct->setStatusTip(tr("Delete all"));
    connect(deleteAllAct, SIGNAL(triggered()), m_canvasWidget, SLOT(deleteAllItemsSlot()));

    QActionGroup* backgroundColorGroup = new QActionGroup(this);
    m_BgColorWhiteAction = new QAction(QIcon(":/images/white.svg"), tr("&White"), this);
    m_BgColorWhiteAction->setCheckable(true);

    m_BgColorGrayAction = new QAction(QIcon(":/images/gray.svg"), tr("&Gray"), this);
    m_BgColorGrayAction->setCheckable(true);

    m_BgColorCyanAction = new QAction(QIcon(":/images/cyan.svg"), tr("&Cyan"), this);
    m_BgColorCyanAction->setCheckable(true);

    m_BgDefaultImage1Action = new QAction(QIcon(":/images/default_image1.svg"), tr("&Default background image titles"), this);
    m_BgDefaultImage1Action->setCheckable(true);
    m_BgDefaultImage1Action->setChecked(true);

    m_BgDefaultImage2Action = new QAction(QIcon(":/images/default_image2.svg"), tr("&Default background image painting"), this);
    m_BgDefaultImage2Action->setCheckable(true);

    m_BgImageKanban1Action = new QAction(QIcon(":/images/image_kanban_1.svg"), tr("&Background image Kanban 1"), this);
    m_BgImageKanban1Action->setCheckable(true);
    m_BgImageKanban1HAction = new QAction(QIcon(":/images/image_kanban_1h.svg"), tr("&Background image Kanban 1H"), this);
    m_BgImageKanban1HAction->setCheckable(true);
    m_BgImageKanban2Action = new QAction(QIcon(":/images/image_kanban_2.svg"), tr("&Background image Kanban 2"), this);
    m_BgImageKanban2Action->setCheckable(true);
    m_BgImageKanban2HAction = new QAction(QIcon(":/images/image_kanban_2h.svg"), tr("&Background image Kanban 2H"), this);
    m_BgImageKanban2HAction->setCheckable(true);

    m_BgUserImageAction = new QAction(QIcon(":/images/load_background_image.svg"), tr("&User background image"), this);
    m_BgUserImageAction->setStatusTip(tr("User background image"));
    m_BgUserImageAction->setCheckable(true);
    //connect(m_BgUserImageAction, SIGNAL(triggered()), m_canvasWidget, SLOT(loadBackgroundImageSlot()));

    backgroundColorGroup->addAction(m_BgColorWhiteAction);
    backgroundColorGroup->addAction(m_BgColorGrayAction);
    backgroundColorGroup->addAction(m_BgColorCyanAction);
    backgroundColorGroup->addAction(m_BgDefaultImage1Action);
    backgroundColorGroup->addAction(m_BgDefaultImage2Action);
    backgroundColorGroup->addAction(m_BgImageKanban1Action);
    backgroundColorGroup->addAction(m_BgImageKanban1HAction);
    backgroundColorGroup->addAction(m_BgImageKanban2Action);
    backgroundColorGroup->addAction(m_BgImageKanban2HAction);
    backgroundColorGroup->addAction(m_BgUserImageAction);
    backgroundColorGroup->setExclusive(true);
    connect(backgroundColorGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeBackgroundColorSlot(QAction*)));
}

void MainWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadProjectAct);
    fileMenu->addAction(saveProjectAct);
    fileMenu->addSeparator();
    fileMenu->addAction(loadTextFileAct);
    fileMenu->addAction(exportAsPictureAct);
    fileMenu->addSeparator();
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(deleteAllAct);

    //menuBar()->addMenu(tr("&View"));
    QMenu* backgroundMenu = menuBar()->addMenu(tr("&Background"));
    backgroundMenu->addAction(m_BgColorWhiteAction);
    backgroundMenu->addAction(m_BgColorGrayAction);
    backgroundMenu->addAction(m_BgColorCyanAction);
    backgroundMenu->addSeparator();
    backgroundMenu->addAction(m_BgDefaultImage1Action);
    backgroundMenu->addAction(m_BgDefaultImage2Action);
    backgroundMenu->addSeparator();
    backgroundMenu->addAction(m_BgImageKanban1Action);
    backgroundMenu->addAction(m_BgImageKanban1HAction);
    backgroundMenu->addAction(m_BgImageKanban2Action);
    backgroundMenu->addAction(m_BgImageKanban2HAction);
    backgroundMenu->addSeparator();
    backgroundMenu->addAction(m_BgUserImageAction);

    menuBar()->addSeparator();
    menuBar()->addMenu(tr("&Help"));
}

void MainWindow::loadProjectSlot()
{
    QString sFilename = QFileDialog::getOpenFileName(this, "Load project from file ", QString(),
                                                     tr("Semaphored project files (*.sem)"));
    if(sFilename.size())
        m_canvasWidget->loadProject(sFilename);
}

void MainWindow::saveProjectSlot()
{
    QString sFilename = QFileDialog::getSaveFileName(this, "Save project as: ", "untitled.sem",
                                                     tr("Semaphored project files (*.sem)"));
    if(sFilename.size()) {
        QByteArray ext = QFileInfo(sFilename).suffix().toLower().toLatin1();
        if(ext != "sem")
            sFilename += ".sem";
        m_canvasWidget->saveProject(sFilename);
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

void MainWindow::changeBackgroundColorSlot(QAction * action)
{
    QColor newColor(Qt::white);
    if (action == m_BgColorWhiteAction )
        m_canvasWidget->changeBackgroundColor(Qt::white);
    else if (action == m_BgColorGrayAction)
        m_canvasWidget->changeBackgroundColor(Qt::gray);
    else if (action == m_BgColorCyanAction)
        m_canvasWidget->changeBackgroundColor(newColor = Qt::cyan);
    else if (action == m_BgDefaultImage1Action)
        m_canvasWidget->changeBackgroundImage(DragWidget::BG_IMAGE_DEFAULT_1);
    else if (action == m_BgDefaultImage2Action)
        m_canvasWidget->changeBackgroundImage(DragWidget::BG_IMAGE_DEFAULT_2);
    else if (action == m_BgImageKanban1Action)
        m_canvasWidget->changeBackgroundImage(DragWidget::BG_IMAGE_KANBAN_1);
    else if (action == m_BgImageKanban1HAction)
        m_canvasWidget->changeBackgroundImage(DragWidget::BG_IMAGE_KANBAN_1H);
    else if (action == m_BgImageKanban2Action)
        m_canvasWidget->changeBackgroundImage(DragWidget::BG_IMAGE_KANBAN_2);
    else if (action == m_BgImageKanban2HAction)
        m_canvasWidget->changeBackgroundImage(DragWidget::BG_IMAGE_KANBAN_2H);
    else if (action == m_BgUserImageAction)
        m_canvasWidget->loadUserBackgroundImage();
  }

void MainWindow::loadTextFileSlot()
{
    QString sFilename = QFileDialog::getOpenFileName(this, "Load source test file: ",QString(),
                                                     tr("Text files (*.txt *.*)"));
    if(sFilename.size())
        m_canvasWidget->loadTextFile(sFilename);
}

DragWidget*  MainWindow::canvasWidget()
{
    return m_canvasWidget;
}

void MainWindow::printCurrentCanvas()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dlg(&printer, this);
    if (dlg.exec() != QDialog::Accepted)
      return;

    QPixmap qpm = QPixmap::grabWidget(centralWidget());
    QPainter painter;
    qpm = qpm.scaled(printer.pageRect().width(), printer.pageRect().height(), Qt::KeepAspectRatio);
    painter.begin (&printer);
    painter.drawPixmap (0, 0, qpm);
    painter.end();
}
