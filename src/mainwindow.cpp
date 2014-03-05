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

#include <QMenuBar>
#include <QFileDialog>
#include <QImageWriter>
#include <QDebug>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QDateTime>
#include <QGridLayout>
#include <QEvent>
#include <QPair>
#include <QTimer>

#include "mainwindow.h"
#include "draglabel.h"
#include "dragwidget.h"
#include "aboutdialog.h"
#include "version.h"
#include "commandlineargs.h"
#include "desktopfile.h"

namespace {
    MainWindow * g_pMainGuiWindow = NULL;
}


class MainWindow_private : public QObject
{
  public:
    explicit MainWindow_private(QObject* parent) :
            QObject(parent),
      loadProjectAct(NULL),
      loadProjectInNewInstanceAct(NULL),
      reloadProjectAct(NULL),
      saveProjectAct(NULL),
      backupProjectWithTimeStampAct(NULL),
      saveProjectAsAct(NULL),
      exportAsPictureAct(NULL),
      loadTextFileAct(NULL),
      printAct(NULL),
      exportAsPdf(NULL),
      quitAct(NULL),
      deleteAllAct(NULL),
      createDesktopLinkAct(NULL),
      aboutAct(NULL),
      m_canvasWidget(new DragWidget()), //TODO use size hint in  canvas
      m_aboutDialog(NULL),
      m_sOpenedFile(""),
      intervalReloadTimer(NULL)
    {}

        QAction* loadProjectAct;
        QAction* loadProjectInNewInstanceAct;
        QAction* reloadProjectAct;
        QAction* saveProjectAct;
        QAction* backupProjectWithTimeStampAct;
        QAction* saveProjectAsAct;
        QAction* exportAsPictureAct;
        QAction* loadTextFileAct;
        QAction* printAct;
        QAction* exportAsPdf;
        QAction* quitAct;
        QAction* deleteAllAct;
        QAction* m_BgColorWhiteAction;
        QAction* m_BgColorGrayAction ;
        QAction* m_BgColorCyanAction;
        QAction* m_BgDefaultImage1Action;
        QAction* m_BgDefaultImage2Action;
        QAction* m_BgImageKanban1Action;
        QAction* m_BgImageKanban1HAction;
        QAction* m_BgImageKanban2Action;
        QAction* m_BgImageKanban2HAction;
        QAction* m_BgImageKanban3Action;
        QAction* m_BgImageKanban3HAction;
        QAction* m_BgImageReview1Action;
        QAction* m_BgImageReview2Action;
        QAction* m_BgUserImageAction;
        QAction* m_BgFixedSize;
        QAction* m_BgSizeDefault;
        QAction* m_BgSize_A5_portrait;
        QAction* m_BgSize_A5_landscape;
        QAction* m_BgSize_A4_portrait;
        QAction* m_BgSize_A4_landscape;
        QAction* m_BgSize_800_600;
        QAction* m_BgSize_1024_768;
        QAction* m_BgSize_1280_768;
        QAction* createDesktopLinkAct;
        QMap<int, QAction*> automaticReloadIntervalActMap;
        QAction* aboutAct;
        DragWidget* m_canvasWidget;
        AboutDialog* m_aboutDialog;
        QString m_sOpenedFile;
        QSystemTrayIcon* m_pTrayIcon;
        QTimer *intervalReloadTimer;

};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    d(new MainWindow_private(this))
{
    g_pMainGuiWindow = this;
    //createDockWindows();
    createActions();
    createMenus();
    //createToolBars();
    //createStatusBar();
    setCentralWidget(d->m_canvasWidget);
#ifdef DEBUG
    setWindowTitle(QString(APPLICATION_NAME) + "-" + QString(APP_FULL_VERSION_TAG) + " (" + QString(APP_DATE) +")");
#else
    setWindowTitle(QString(APPLICATION_NAME) + "-" + QString(APP_SHORT_VERSION_TAG));
#endif //DEBUG

    setWindowIcon(QIcon(":/icons/semaphored.svg"));

    d->m_pTrayIcon = new QSystemTrayIcon(QIcon(":/icons/semaphored.svg"),this);
    connect(this, SIGNAL(signalPlaceToTray()),this, SLOT(slotPlaceToTray()),Qt::QueuedConnection);
    connect(d->m_pTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    d->m_pTrayIcon->show();
}

MainWindow* MainWindow::instance()
{
    return  g_pMainGuiWindow;
}

void MainWindow::createActions()
{
    d->loadProjectAct = new QAction(QIcon(":/icons/load_project.svg"), tr("L&oad project.."), this);
    d->loadProjectAct->setStatusTip(tr("Load project from file"));
    d->loadProjectAct->setShortcut(tr("Ctrl+O"));
    connect(d->loadProjectAct, SIGNAL(triggered()), this, SLOT(loadProjectSlot()));

    d->loadProjectInNewInstanceAct = new QAction(QIcon(":/icons/load_project.svg"), tr("Load project in new &instance.."), this);
    d->loadProjectInNewInstanceAct->setStatusTip(tr("Load project in new instance"));
    d->loadProjectInNewInstanceAct->setShortcut(tr("Shift+Ctrl+O"));
    connect(d->loadProjectInNewInstanceAct, SIGNAL(triggered()), this, SLOT(loadProjectInNewInstanceSlot()));

    d->reloadProjectAct = new QAction(QIcon(":/icons/reload_project.svg"), tr("&Reload project"), this);
    d->reloadProjectAct->setStatusTip(tr("Reload project.."));
    d->reloadProjectAct->setShortcut(tr("Ctrl+R"));
    connect(d->reloadProjectAct, SIGNAL(triggered()), this, SLOT(reloadProjectSlot()));

    d->saveProjectAct = new QAction(QIcon(":/icons/save_project.svg"), tr("&Save project"), this);
    d->saveProjectAct->setStatusTip(tr("Save project"));
    d->saveProjectAct->setShortcut(tr("Ctrl+S"));
    connect(d->saveProjectAct, SIGNAL(triggered()), this, SLOT(saveProjectSlot()));

    d->saveProjectAsAct = new QAction(QIcon(":/icons/save_project_as.svg"), tr("Save project &as .."), this);
    d->saveProjectAsAct->setStatusTip(tr("Save project to different file"));
    d->saveProjectAsAct->setShortcut(tr("Ctrl+Shift+S"));
    connect(d->saveProjectAsAct, SIGNAL(triggered()), this, SLOT(saveProjectAsSlot()));

    d->backupProjectWithTimeStampAct = new QAction(QIcon(":/icons/backup_project_ts.svg"), tr("&Backup project with timestamp"), this);
    d->backupProjectWithTimeStampAct->setStatusTip(tr("Backup project with timestamp"));
    d->backupProjectWithTimeStampAct->setShortcut(tr("Ctrl+B"));
    connect(d->backupProjectWithTimeStampAct, SIGNAL(triggered()), this, SLOT(backupProjectWithTimeStampSlot()));

    d->exportAsPictureAct = new QAction(QIcon(":/icons/export_as_picture.png"), tr("&Export as a picture.."), this);
    d->exportAsPictureAct->setStatusTip(tr("Export as a picture"));
    d->exportAsPictureAct->setShortcut(tr("Ctrl+E"));
    connect(d->exportAsPictureAct, SIGNAL(triggered()), this, SLOT(exportAsPictureSlot()));

    d->loadTextFileAct = new QAction(QIcon(":/icons/load_text_file.svg"), tr("&Load text file.."), this);
    d->loadTextFileAct->setStatusTip(tr("Load text file"));
    d->loadTextFileAct->setShortcut(tr("Ctrl+L"));
    connect(d->loadTextFileAct, SIGNAL(triggered()), this, SLOT(loadTextFileSlot()));

    d->exportAsPdf = new QAction(QIcon(":/icons/export_as_pdf.svg"), tr("Export &To PDF.."), this);
    d->exportAsPdf->setStatusTip(tr("Export To PDF"));
    d->exportAsPdf->setShortcut(tr("Ctrl+Shift+P"));
    connect(d->exportAsPdf, SIGNAL(triggered()), this, SLOT(exportCanvasToPdfSlot()));

    d->printAct = new QAction(QIcon(":/icons/print.svg"), tr("&Print.."), this);
    d->printAct->setShortcuts(QKeySequence::Print);
    d->printAct->setStatusTip(tr("Print the current canvas"));
    connect(d->printAct, SIGNAL(triggered()), this, SLOT(printCurrentCanvasSlot()));

    d->quitAct = new QAction(QIcon(":/icons/quit.svg"), tr("&Quit"), this);
    d->quitAct->setStatusTip(tr("Quit"));
    d->quitAct->setShortcut(tr("Ctrl+Q"));
    connect(d->quitAct, SIGNAL(triggered()), this, SLOT(close()));

    d->deleteAllAct = new QAction(QIcon(":/icons/delete_all.svg"), tr("D&elete All"), this);
    d->deleteAllAct->setStatusTip(tr("Delete all"));
    d->deleteAllAct->setShortcut(tr("Ctrl+E"));
    connect(d->deleteAllAct, SIGNAL(triggered()), d->m_canvasWidget, SLOT(deleteAllItemsSlot()));

    QActionGroup* backgroundColorGroup = new QActionGroup(this);
    d->m_BgColorWhiteAction = new QAction(QIcon(":/icons/white-bg.svg"), tr("&White"), this);
    d->m_BgColorWhiteAction->setCheckable(true);

    d->m_BgColorGrayAction = new QAction(QIcon(":/icons/gray-bg.svg"), tr("&Gray"), this);
    d->m_BgColorGrayAction->setCheckable(true);

    d->m_BgColorCyanAction = new QAction(QIcon(":/icons/cyan-bg.svg"), tr("&Cyan"), this);
    d->m_BgColorCyanAction->setCheckable(true);

    d->m_BgDefaultImage1Action = new QAction(QIcon(":/icons/default_image1.png"), tr("&Default background image titles"), this);
    d->m_BgDefaultImage1Action->setCheckable(true);
    d->m_BgDefaultImage1Action->setChecked(true);

    d->m_BgDefaultImage2Action = new QAction(QIcon(":/icons/default_image2.png"), tr("&Default background image painting"), this);
    d->m_BgDefaultImage2Action->setCheckable(true);

    d->m_BgImageKanban1Action = new QAction(QIcon(":/icons/bg-kanban1-a5.svg"), tr("&Kanban BTID 4 cols"), this);
    d->m_BgImageKanban1Action->setCheckable(true);
    d->m_BgImageKanban1HAction = new QAction(QIcon(":/icons/bg-kanban1-a5h.svg"), tr("&Kanban BTID 4 rows"), this);
    d->m_BgImageKanban1HAction->setCheckable(true);
    d->m_BgImageKanban2Action = new QAction(QIcon(":/icons/bg-kanban2-a5.svg"), tr("&Kanban TID 3 cols"), this);
    d->m_BgImageKanban2Action->setCheckable(true);
    d->m_BgImageKanban2HAction = new QAction(QIcon(":/icons/bg-kanban2-a5h.svg"), tr("&Kanban TID 3 rows"), this);
    d->m_BgImageKanban2HAction->setCheckable(true);
    d->m_BgImageKanban3Action = new QAction(QIcon(":/icons/bg-kanban3-a5.svg"), tr("&Kanban NSID 4 cols"), this);
    d->m_BgImageKanban3Action->setCheckable(true);
    d->m_BgImageKanban3HAction = new QAction(QIcon(":/icons/bg-kanban3-a5h.svg"), tr("&Kanban NSID 4 rows"), this);
    d->m_BgImageKanban3HAction->setCheckable(true);

    d->m_BgImageReview1Action = new QAction(QIcon(":/icons/bg-review1-a5.svg"), tr("&Review board 4 cols"), this);
    d->m_BgImageReview1Action->setCheckable(true);
    d->m_BgImageReview2Action = new QAction(QIcon(":/icons/bg-review2-a5.svg"), tr("&Double review board 6 cols"), this);
    d->m_BgImageReview2Action->setCheckable(true);


    d->m_BgUserImageAction = new QAction(QIcon(":/icons/load_background_image.png"), tr("&User background image"), this);
    d->m_BgUserImageAction->setStatusTip(tr("User background image"));
    d->m_BgUserImageAction->setCheckable(true);
    //connect(m_BgUserImageAction, SIGNAL(triggered()), m_canvasWidget, SLOT(loadBackgroundImageSlot()));

    backgroundColorGroup->addAction(d->m_BgColorWhiteAction);
    backgroundColorGroup->addAction(d->m_BgColorGrayAction);
    backgroundColorGroup->addAction(d->m_BgColorCyanAction);
    backgroundColorGroup->addAction(d->m_BgDefaultImage1Action);
    backgroundColorGroup->addAction(d->m_BgDefaultImage2Action);
    backgroundColorGroup->addAction(d->m_BgImageKanban1Action);
    backgroundColorGroup->addAction(d->m_BgImageKanban1HAction);
    backgroundColorGroup->addAction(d->m_BgImageKanban2Action);
    backgroundColorGroup->addAction(d->m_BgImageKanban2HAction);
    backgroundColorGroup->addAction(d->m_BgImageKanban3Action);
    backgroundColorGroup->addAction(d->m_BgImageKanban3HAction);
    backgroundColorGroup->addAction(d->m_BgImageReview1Action);
    backgroundColorGroup->addAction(d->m_BgImageReview2Action);
    backgroundColorGroup->addAction(d->m_BgUserImageAction);
    backgroundColorGroup->setExclusive(true);
    connect(backgroundColorGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeBackgroundColorSlot(QAction*)));

    QActionGroup* backgroundSizeGroup = new QActionGroup(this);
    d->m_BgFixedSize = new QAction(QIcon(":/icons/size.png"), tr("&Fixed size"), this);
    d->m_BgFixedSize->setCheckable(true);
    connect(d->m_BgFixedSize, SIGNAL(triggered ( bool )), canvasWidget(), SLOT(setFixedSizeBg(bool )));
    connect(canvasWidget(), SIGNAL(changeFixedSize ( bool )), d->m_BgFixedSize, SLOT(setChecked(bool )));


    d->m_BgSizeDefault = new QAction(QIcon(":/icons/size.png"), tr("&Default backgound size"), this);
    d->m_BgSizeDefault->setCheckable(true);

    d->m_BgSize_A5_landscape = new QAction(QIcon(":/icons/size.png"), tr("&A5 landscape"), this);
    d->m_BgSize_A5_landscape->setCheckable(true);

    d->m_BgSize_A5_portrait = new QAction(QIcon(":/icons/size.png"), tr("&A5 portrait"), this);
    d->m_BgSize_A5_portrait->setCheckable(true);

    d->m_BgSize_A4_landscape = new QAction(QIcon(":/icons/size.png"), tr("&A4 landscape"), this);
    d->m_BgSize_A4_landscape->setCheckable(true);

    d->m_BgSize_A4_portrait = new QAction(QIcon(":/icons/size.png"), tr("&A4 portrait"), this);
    d->m_BgSize_A4_portrait->setCheckable(true);

    d->m_BgSize_800_600 = new QAction(QIcon(":/icons/size.png"), tr("&800x600"), this);
    d->m_BgSize_800_600->setCheckable(true);

    d->m_BgSize_1024_768 = new QAction(QIcon(":/icons/size.png"), tr("&1024x768"), this);
    d->m_BgSize_1024_768->setCheckable(true);

    d->m_BgSize_1280_768 = new QAction(QIcon(":/icons/size.png"), tr("&1280x768"), this);
    d->m_BgSize_1280_768->setCheckable(true);

    backgroundSizeGroup->addAction(d->m_BgSizeDefault);
    backgroundSizeGroup->addAction(d->m_BgSize_A5_landscape);
    backgroundSizeGroup->addAction(d->m_BgSize_A5_portrait);
    backgroundSizeGroup->addAction(d->m_BgSize_A4_landscape);
    backgroundSizeGroup->addAction(d->m_BgSize_A4_portrait);
    backgroundSizeGroup->addAction(d->m_BgSize_800_600);
    backgroundSizeGroup->addAction(d->m_BgSize_1024_768);
    backgroundSizeGroup->addAction(d->m_BgSize_1280_768);
    connect(backgroundSizeGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeBackgroundSizeSlot(QAction*)));

    QActionGroup* reloadIntervalGroup = new QActionGroup(this);
    reloadIntervalGroup->setExclusive(true);
    connect(reloadIntervalGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeIntervalReloadSlot(QAction*)));

    QAction* intervalReloadAct(NULL);
    typedef QPair<int, QString> TimeInterval;
    foreach (const TimeInterval &interval, QList<TimeInterval>()
             << qMakePair(   0, tr("OFF"))   << qMakePair(  10, tr("10s"))
             << qMakePair(  60, tr("1min"))  << qMakePair( 300, tr("5min"))
             << qMakePair( 900, tr("15min")) << qMakePair(3600, tr("1h"))) {
        intervalReloadAct = new QAction(interval.second, this);
        intervalReloadAct->setCheckable(true);
        if(interval.first == 0) //default is off
            intervalReloadAct->setChecked(true);
        d->automaticReloadIntervalActMap.insert(interval.first, intervalReloadAct );
        reloadIntervalGroup->addAction(intervalReloadAct);
    }

    d->createDesktopLinkAct = new QAction(QIcon(":/icons/create_link.svg"), tr("&Create desktop link"), this);
    d->createDesktopLinkAct->setStatusTip(tr("Create desktop link"));
    connect(d->createDesktopLinkAct, SIGNAL(triggered()), this, SLOT(createDesktopLinkSlot()));

    d->aboutAct = new QAction(QIcon(":/icons/about.svg"), tr("&About"), this);
    d->aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(d->aboutAct, SIGNAL(triggered()), this, SLOT(showAboutDialogSlot()));
}

void MainWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(d->loadProjectAct);
    fileMenu->addAction(d->loadProjectInNewInstanceAct);
    fileMenu->addAction(d->reloadProjectAct);
    fileMenu->addSeparator();
    fileMenu->addAction(d->saveProjectAct);
    fileMenu->addAction(d->backupProjectWithTimeStampAct);
    fileMenu->addAction(d->saveProjectAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(d->loadTextFileAct);
    fileMenu->addSeparator();
    fileMenu->addAction(d->exportAsPictureAct);
    fileMenu->addAction(d->exportAsPdf);
    fileMenu->addAction(d->printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(d->quitAct);

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(d->deleteAllAct);
    //menuBar()->addMenu(tr("&View"));
    QMenu* backgroundMenu = menuBar()->addMenu(tr("&Background"));
    backgroundMenu->addAction(d->m_BgColorWhiteAction);
    backgroundMenu->addAction(d->m_BgColorGrayAction);
    backgroundMenu->addAction(d->m_BgColorCyanAction);
    backgroundMenu->addSeparator();
    QMenu* defaultImagesMenu = backgroundMenu->addMenu(tr("&Default images"));
    defaultImagesMenu->addAction(d->m_BgDefaultImage1Action);
    defaultImagesMenu->addAction(d->m_BgDefaultImage2Action);
    QMenu* kanbanImagesMenu = backgroundMenu->addMenu(tr("&Kanban dashboards"));
    kanbanImagesMenu->addAction(d->m_BgImageKanban1Action);
    kanbanImagesMenu->addAction(d->m_BgImageKanban1HAction);
    kanbanImagesMenu->addAction(d->m_BgImageKanban2Action);
    kanbanImagesMenu->addAction(d->m_BgImageKanban2HAction);
    kanbanImagesMenu->addAction(d->m_BgImageKanban3Action);
    kanbanImagesMenu->addAction(d->m_BgImageKanban3HAction);
    QMenu* reviewImagesMenu = backgroundMenu->addMenu(tr("&Review dashboards"));
    reviewImagesMenu->addAction(d->m_BgImageReview1Action);
    reviewImagesMenu->addAction(d->m_BgImageReview2Action);
    backgroundMenu->addSeparator();
    backgroundMenu->addAction(d->m_BgUserImageAction);
    backgroundMenu->addSeparator();
    QMenu* sizeMenu = backgroundMenu->addMenu(QIcon(":/icons/sizes.svg"), tr("&Size"));
    sizeMenu->addAction(d->m_BgSizeDefault);
    sizeMenu->addAction(d->m_BgSize_A5_landscape);
    sizeMenu->addAction(d->m_BgSize_A5_portrait);
    sizeMenu->addAction(d->m_BgSize_A4_landscape);
    sizeMenu->addAction(d->m_BgSize_A4_portrait);
    sizeMenu->addAction(d->m_BgSize_800_600);
    sizeMenu->addAction(d->m_BgSize_1024_768);
    sizeMenu->addAction(d->m_BgSize_1280_768);
    backgroundMenu->addAction(d->m_BgFixedSize);

    menuBar()->addSeparator();
    QMenu* settingsMenu =  menuBar()->addMenu(tr("&Settings"));
    QMenu* automaticReloadMenu =settingsMenu->addMenu("Automatic project reload");
    foreach (QAction* action, d->automaticReloadIntervalActMap)
        automaticReloadMenu->addAction(action);
    settingsMenu->addSeparator();
    settingsMenu->addAction(d->createDesktopLinkAct);

    QMenu* helpMenu =  menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(d->aboutAct);
}
void MainWindow::loadProjectSlot()
{
    QString sFilename = QFileDialog::getOpenFileName(this, "Load project from file ", QString(),
                                                 tr("Semaphored project files (*.sem);;All files (*)"));
    if(sFilename.size())
       if(QMessageBox::question(this, tr("Load project"),
                                             tr("Do you want to <b>load project</b> %1 ? \nUnsaved changes in current project will be lost.").arg(sFilename),
                                             QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel) == QMessageBox::Yes)
            loadProject(sFilename);
}

void MainWindow::loadProjectInNewInstanceSlot()
{
    QString sFilename = QFileDialog::getOpenFileName(this, "Load project in new instance from file ", QString(),
                                                 tr("Semaphored project files (*.sem);;All files (*)"));
    if (sFilename.size())
        CommandLineArgs::getInstance()->startNewInstance(sFilename);
}

void MainWindow::loadProject(const QString& sFilename)
{
    if (!sFilename.startsWith(":/")) { //files from resources
      d->m_sOpenedFile = sFilename;
      setWindowTitle(d->m_sOpenedFile);
      //TODO better logic, open file, new file
    }
    canvasWidget()->loadProject(sFilename);
}

void MainWindow::reloadProjectSlot()
{
    if (d->m_sOpenedFile.size()) //we load only real files
        canvasWidget()->loadProject(d->m_sOpenedFile);
}

void MainWindow::saveProjectSlot()
{
    if (!d->m_sOpenedFile.isEmpty())
        canvasWidget()->saveProject(d->m_sOpenedFile);
    else
        saveProjectAsSlot();
}

void MainWindow::saveProjectAsSlot()
{
    QString sFilename = QFileDialog::getSaveFileName(this, "Save project as: ",
                        d->m_sOpenedFile.size() ? d->m_sOpenedFile : "untitled.sem",
                        tr("Semaphored project files (*.sem);;All files (*)"));
    if(sFilename.size()) {
        QByteArray ext = QFileInfo(sFilename).suffix().toLower().toLatin1();
        if(ext != "sem")
            sFilename += ".sem";
        d->m_sOpenedFile = sFilename;
        setWindowTitle(d->m_sOpenedFile);
        canvasWidget()->saveProject(d->m_sOpenedFile);
    }
}

void MainWindow::backupProjectWithTimeStampSlot()
{
    if(!d->m_sOpenedFile.isEmpty()) {
        QString sBackupFile = d->m_sOpenedFile;
        sBackupFile.chop(4); //
        sBackupFile += QDateTime::currentDateTime().toString("-yyyy_MM_dd-hh_mm");
        sBackupFile += ".sem";
        canvasWidget()->saveProject(sBackupFile);
    }
}

void MainWindow::exportAsPictureSlot()
{
    QString supportedFormats("");
    QString supportedFormatsFilter("Images (");
    foreach(QByteArray name,QImageWriter::supportedImageFormats())
        supportedFormats +=  name + " ";
    supportedFormatsFilter+= "));;SVG files (*.svg);;All files (*)";
    QString sOpenedFileBasename = d->m_sOpenedFile.size() ? QFileInfo(d->m_sOpenedFile).baseName() : "untitled";


    QString sFilename = QFileDialog::getSaveFileName(this, "Save as picture as: " + supportedFormats, sOpenedFileBasename + ".png",
                                                     supportedFormatsFilter);
    if(sFilename.size()) {
        QByteArray ext = QFileInfo(sFilename).suffix().toLower().toLatin1();
        if (ext == "svg" || QImageWriter::supportedImageFormats().contains(ext)) {
            canvasWidget()->exportToPicture(sFilename);
        } else {
            QMessageBox::warning(this,"Picture was not exported", "Your system supports only following formats: " + supportedFormats);
        }
    }
}

void MainWindow::changeBackgroundColorSlot(QAction * action)
{
   //TPDP this could be unified by m_BackgroundPicture, "" - default, filename = picture, QColor::isValidColor  - color
    QColor newColor(Qt::white);
    if (action == d->m_BgColorWhiteAction )
        canvasWidget()->changeBackgroundColor(Qt::white);
    else if (action == d->m_BgColorGrayAction)
        canvasWidget()->changeBackgroundColor(Qt::gray);
    else if (action == d->m_BgColorCyanAction)
        canvasWidget()->changeBackgroundColor(newColor = Qt::cyan);
    else if (action == d->m_BgDefaultImage1Action)
        canvasWidget()->changeBackgroundImage(DragWidget::BG_IMAGE_DEFAULT_1);
    else if (action == d->m_BgDefaultImage2Action)
        canvasWidget()->changeBackgroundImage(DragWidget::BG_IMAGE_DEFAULT_2);
    else if (action == d->m_BgImageKanban1Action)
        canvasWidget()->changeBackgroundImage(DragWidget::BG_IMAGE_KANBAN_1);
    else if (action == d->m_BgImageKanban1HAction)
        canvasWidget()->changeBackgroundImage(DragWidget::BG_IMAGE_KANBAN_1H);
    else if (action == d->m_BgImageKanban2Action)
        canvasWidget()->changeBackgroundImage(DragWidget::BG_IMAGE_KANBAN_2);
   else if (action == d->m_BgImageKanban2HAction)
        canvasWidget()->changeBackgroundImage(DragWidget::BG_IMAGE_KANBAN_2H);
    else if (action == d->m_BgImageKanban3Action)
        canvasWidget()->changeBackgroundImage(DragWidget::BG_IMAGE_KANBAN_3);
   else if (action == d->m_BgImageKanban3HAction)
        canvasWidget()->changeBackgroundImage(DragWidget::BG_IMAGE_KANBAN_3H);
    else if (action == d->m_BgImageReview1Action)
         canvasWidget()->changeBackgroundImage(DragWidget::BG_IMAGE_REVIEW_1);
    else if (action == d->m_BgImageReview2Action)
         canvasWidget()->changeBackgroundImage(DragWidget::BG_IMAGE_REVIEW_2);
    else if (action == d->m_BgUserImageAction) {
        canvasWidget()->loadUserBackgroundImage();
   }
}

void MainWindow::changeBackgroundSizeSlot(QAction * action)
{
    QSize newSize(canvasWidget()->size());
    if (action == d->m_BgSizeDefault)
        ;
    else if (action == d->m_BgSize_A5_portrait)
        newSize = QSize(DragWidget::SIZE_A5_SHORT,DragWidget::SIZE_A5_LONG);
    else if (action == d->m_BgSize_A5_landscape)
        newSize = QSize(DragWidget::SIZE_A5_LONG,DragWidget::SIZE_A5_SHORT);
    else if (action == d->m_BgSize_A4_portrait)
        newSize = QSize(DragWidget::SIZE_A4_SHORT,DragWidget::SIZE_A4_LONG);
    else if (action == d->m_BgSize_A4_landscape)
        newSize = QSize(DragWidget::SIZE_A4_LONG,DragWidget::SIZE_A4_SHORT);
    else if (action == d->m_BgSize_800_600)
        newSize = QSize(800,600);
    else if (action == d->m_BgSize_1024_768)
        newSize = QSize(1024,768);
    else if (action == d->m_BgSize_1280_768)
        newSize = QSize(1280,768);
    resize(newSize);
}

void MainWindow::loadTextFileSlot()
{
    QString sFilename = QFileDialog::getOpenFileName(this, "Load source test file: ",QString(),
                                                     tr("Text files (*.txt *.*)"));
    if(sFilename.size())
        canvasWidget()->loadTextFile(sFilename);
}

void MainWindow::exportToTextFileSlot()
{
    QString sFilename = QFileDialog::getOpenFileName(this, "Load source test file: ",QString(),
                                                     tr("Text files (*.txt *.*)"));
    if(sFilename.size())
        canvasWidget()->loadTextFile(sFilename);
}


void MainWindow::changeIntervalReloadSlot(QAction * action)
{
    int i = d->automaticReloadIntervalActMap.key(action, 0);
    if(i) {
        if (!d->intervalReloadTimer) {
            d->intervalReloadTimer = new QTimer(this);
            connect(d->intervalReloadTimer, SIGNAL(timeout()), this, SLOT(intervalReloadSlot()));
        }
        d->intervalReloadTimer->setInterval(i*1000);
        d->intervalReloadTimer->start();
    } else if (d->intervalReloadTimer) {
        d->intervalReloadTimer->stop();
    }
}

void MainWindow::intervalReloadSlot()
{
    if (!d->m_sOpenedFile.isEmpty())
        loadProject(d->m_sOpenedFile);
}

DragWidget*  MainWindow::canvasWidget()
{
    return d->m_canvasWidget;
}

void MainWindow::printCurrentCanvasSlot()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dlg(&printer, this);
    if (dlg.exec() != QDialog::Accepted)
      return;

    QPixmap qpm = QPixmap::grabWidget(canvasWidget());
    QPainter painter;
    qpm = qpm.scaled(printer.pageRect().width(), printer.pageRect().height(), Qt::KeepAspectRatio);
    painter.begin (&printer);
    painter.drawPixmap (0, 0, qpm);
    painter.end();
}

void MainWindow::exportCanvasToPdfSlot()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export to PDF", QString(), "*.pdf");
    if (!fileName.isEmpty()) {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);

        QPixmap qpm = QPixmap::grabWidget(canvasWidget());
        QPainter painter;
        qpm = qpm.scaled(printer.pageRect().width(), printer.pageRect().height(), Qt::KeepAspectRatio);
        painter.begin (&printer);
        painter.drawPixmap (0, 0, qpm);
        painter.end();
    }
}

void MainWindow::createDesktopLinkSlot() {
    DesktopFile::CreateDesktopFile();
}

void MainWindow::showAboutDialogSlot()
{
    if (!d->m_aboutDialog) {
        d->m_aboutDialog = new AboutDialog(this);
    }
    d->m_aboutDialog->show();
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        if (isMinimized()) {
              emit signalPlaceToTray();
              event->ignore();
              return;
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::slotPlaceToTray()
{
    hide();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
            if (isVisible())
              hide();
            else
              showNormal();
            break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
            ;
    }
}
