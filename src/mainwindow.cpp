/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://semaphored.safrm.net/
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
#include <QDateTime>
#include <QGridLayout>
#include <QEvent>
#include <QPair>

#include "mainwindow.h"
#include "draglabel.h"
#include "dragwidget.h"
#include "aboutdialog.h"
#include "version.h"
#include "commandlineargs.h"
#include "desktopfile.h"

MainWindow * g_pMainGuiWindow =NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
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
    m_sOpenedFile("")
{
    g_pMainGuiWindow = this;
    //createDockWindows();
    createActions();
    createMenus();
    //createToolBars();
    //createStatusBar();
    setCentralWidget(m_canvasWidget);
#ifdef DEBUG
    setWindowTitle(QString(APPLICATION_NAME) + "-" + QString(APP_FULL_VERSION_TAG) + " (" + QString(APP_DATE) +")");
#else
    setWindowTitle(QString(APPLICATION_NAME) + "-" + QString(APP_SHORT_VERSION_TAG));
#endif //DEBUG

    setWindowIcon(QIcon(":/icons/semaphored.svg"));

    m_pTrayIcon = new QSystemTrayIcon(QIcon(":/icons/semaphored.svg"),this);
    connect(this, SIGNAL(signalPlaceToTray()),this, SLOT(slotPlaceToTray()),Qt::QueuedConnection);
    connect(m_pTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    m_pTrayIcon->show();
}

MainWindow* MainWindow::instance()
{
    return  g_pMainGuiWindow;
}

void MainWindow::createActions()
{
    loadProjectAct = new QAction(QIcon(":/icons/load_project.svg"), tr("L&oad project.."), this);
    loadProjectAct->setStatusTip(tr("Load project from file"));
    loadProjectAct->setShortcut(tr("Ctrl+O"));
    connect(loadProjectAct, SIGNAL(triggered()), this, SLOT(loadProjectSlot()));

    loadProjectInNewInstanceAct = new QAction(QIcon(":/icons/load_project.svg"), tr("Load project in new &instance.."), this);
    loadProjectInNewInstanceAct->setStatusTip(tr("Load project in new instance"));
    loadProjectInNewInstanceAct->setShortcut(tr("Shift+Ctrl+O"));
    connect(loadProjectInNewInstanceAct, SIGNAL(triggered()), this, SLOT(loadProjectInNewInstanceSlot()));

    reloadProjectAct = new QAction(QIcon(":/icons/reload_project.svg"), tr("&Reload project"), this);
    reloadProjectAct->setStatusTip(tr("Reload project.."));
    reloadProjectAct->setShortcut(tr("Ctrl+R"));
    connect(reloadProjectAct, SIGNAL(triggered()), this, SLOT(reloadProjectSlot()));

    saveProjectAct = new QAction(QIcon(":/icons/save_project.svg"), tr("&Save project"), this);
    saveProjectAct->setStatusTip(tr("Save project"));
    saveProjectAct->setShortcut(tr("Ctrl+S"));
    connect(saveProjectAct, SIGNAL(triggered()), this, SLOT(saveProjectSlot()));

    saveProjectAsAct = new QAction(QIcon(":/icons/save_project_as.svg"), tr("Save project &as .."), this);
    saveProjectAsAct->setStatusTip(tr("Save project to different file"));
    saveProjectAsAct->setShortcut(tr("Ctrl+Shift+S"));
    connect(saveProjectAsAct, SIGNAL(triggered()), this, SLOT(saveProjectAsSlot()));

    backupProjectWithTimeStampAct = new QAction(QIcon(":/icons/backup_project_ts.svg"), tr("&Backup project with timestamp"), this);
    backupProjectWithTimeStampAct->setStatusTip(tr("Backup project with timestamp"));
    backupProjectWithTimeStampAct->setShortcut(tr("Ctrl+B"));
    connect(backupProjectWithTimeStampAct, SIGNAL(triggered()), this, SLOT(backupProjectWithTimeStampSlot()));

    exportAsPictureAct = new QAction(QIcon(":/icons/export_as_picture.png"), tr("&Export as a picture.."), this);
    exportAsPictureAct->setStatusTip(tr("Export as a picture"));
    exportAsPictureAct->setShortcut(tr("Ctrl+E"));
    connect(exportAsPictureAct, SIGNAL(triggered()), this, SLOT(exportAsPictureSlot()));

    loadTextFileAct = new QAction(QIcon(":/icons/load_text_file.svg"), tr("&Load text file.."), this);
    loadTextFileAct->setStatusTip(tr("Load text file"));
    loadTextFileAct->setShortcut(tr("Ctrl+L"));
    connect(loadTextFileAct, SIGNAL(triggered()), this, SLOT(loadTextFileSlot()));

    exportAsPdf = new QAction(QIcon(":/icons/export_as_pdf.svg"), tr("Export &To PDF.."), this);
    exportAsPdf->setStatusTip(tr("Export To PDF"));
    exportAsPdf->setShortcut(tr("Ctrl+Shift+P"));
    connect(exportAsPdf, SIGNAL(triggered()), this, SLOT(exportCanvasToPdfSlot()));

    printAct = new QAction(QIcon(":/icons/print.svg"), tr("&Print.."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the current canvas"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(printCurrentCanvasSlot()));

    quitAct = new QAction(QIcon(":/icons/quit.svg"), tr("&Quit"), this);
    quitAct->setStatusTip(tr("Quit"));
    quitAct->setShortcut(tr("Ctrl+Q"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    deleteAllAct = new QAction(QIcon(":/icons/delete_all.svg"), tr("D&elete All"), this);
    deleteAllAct->setStatusTip(tr("Delete all"));
    deleteAllAct->setShortcut(tr("Ctrl+E"));
    connect(deleteAllAct, SIGNAL(triggered()), m_canvasWidget, SLOT(deleteAllItemsSlot()));

    QActionGroup* backgroundColorGroup = new QActionGroup(this);
    m_BgColorWhiteAction = new QAction(QIcon(":/icons/white-bg.svg"), tr("&White"), this);
    m_BgColorWhiteAction->setCheckable(true);

    m_BgColorGrayAction = new QAction(QIcon(":/icons/gray-bg.svg"), tr("&Gray"), this);
    m_BgColorGrayAction->setCheckable(true);

    m_BgColorCyanAction = new QAction(QIcon(":/icons/cyan-bg.svg"), tr("&Cyan"), this);
    m_BgColorCyanAction->setCheckable(true);

    m_BgDefaultImage1Action = new QAction(QIcon(":/icons/default_image1.png"), tr("&Default background image titles"), this);
    m_BgDefaultImage1Action->setCheckable(true);
    m_BgDefaultImage1Action->setChecked(true);

    m_BgDefaultImage2Action = new QAction(QIcon(":/icons/default_image2.png"), tr("&Default background image painting"), this);
    m_BgDefaultImage2Action->setCheckable(true);

    m_BgImageKanban1Action = new QAction(QIcon(":/icons/bg-kanban1-a5.svg"), tr("&Kanban BTID 4 cols"), this);
    m_BgImageKanban1Action->setCheckable(true);
    m_BgImageKanban1HAction = new QAction(QIcon(":/icons/bg-kanban1-a5h.svg"), tr("&Kanban BTID 4 rows"), this);
    m_BgImageKanban1HAction->setCheckable(true);
    m_BgImageKanban2Action = new QAction(QIcon(":/icons/bg-kanban2-a5.svg"), tr("&Kanban TID 3 cols"), this);
    m_BgImageKanban2Action->setCheckable(true);
    m_BgImageKanban2HAction = new QAction(QIcon(":/icons/bg-kanban2-a5h.svg"), tr("&Kanban TID 3 rows"), this);
    m_BgImageKanban2HAction->setCheckable(true);
    m_BgImageKanban3Action = new QAction(QIcon(":/icons/bg-kanban3-a5.svg"), tr("&Kanban NSID 4 cols"), this);
    m_BgImageKanban3Action->setCheckable(true);
    m_BgImageKanban3HAction = new QAction(QIcon(":/icons/bg-kanban3-a5h.svg"), tr("&Kanban NSID 4 rows"), this);
    m_BgImageKanban3HAction->setCheckable(true);

    m_BgImageReview1Action = new QAction(QIcon(":/icons/bg-review1-a5.svg"), tr("&Review board 4 cols"), this);
    m_BgImageReview1Action->setCheckable(true);
    m_BgImageReview2Action = new QAction(QIcon(":/icons/bg-review2-a5.svg"), tr("&Double review board 6 cols"), this);
    m_BgImageReview2Action->setCheckable(true);


    m_BgUserImageAction = new QAction(QIcon(":/icons/load_background_image.png"), tr("&User background image"), this);
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
    backgroundColorGroup->addAction(m_BgImageKanban3Action);
    backgroundColorGroup->addAction(m_BgImageKanban3HAction);
    backgroundColorGroup->addAction(m_BgImageReview1Action);
    backgroundColorGroup->addAction(m_BgImageReview2Action);
    backgroundColorGroup->addAction(m_BgUserImageAction);
    backgroundColorGroup->setExclusive(true);
    connect(backgroundColorGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeBackgroundColorSlot(QAction*)));

    QActionGroup* backgroundSizeGroup = new QActionGroup(this);
    m_BgFixedSize = new QAction(QIcon(":/icons/size.png"), tr("&Fixed size"), this);
    m_BgFixedSize->setCheckable(true);
    connect(m_BgFixedSize, SIGNAL(triggered ( bool )), this, SLOT(changeBackgroundFixed(bool )));
    connect(m_canvasWidget, SIGNAL(changeFixedSize ( bool )), m_BgFixedSize, SLOT(setChecked(bool )));


    m_BgSizeDefault = new QAction(QIcon(":/icons/size.png"), tr("&Default backgound size"), this);
    m_BgSizeDefault->setCheckable(true);

    m_BgSize_A5_landscape = new QAction(QIcon(":/icons/size.png"), tr("&A5 landscape"), this);
    m_BgSize_A5_landscape->setCheckable(true);

    m_BgSize_A5_portrait = new QAction(QIcon(":/icons/size.png"), tr("&A5 portrait"), this);
    m_BgSize_A5_portrait->setCheckable(true);

    m_BgSize_A4_landscape = new QAction(QIcon(":/icons/size.png"), tr("&A4 landscape"), this);
    m_BgSize_A4_landscape->setCheckable(true);

    m_BgSize_A4_portrait = new QAction(QIcon(":/icons/size.png"), tr("&A4 portrait"), this);
    m_BgSize_A4_portrait->setCheckable(true);

    m_BgSize_800_600 = new QAction(QIcon(":/icons/size.png"), tr("&800x600"), this);
    m_BgSize_800_600->setCheckable(true);

    m_BgSize_1024_768 = new QAction(QIcon(":/icons/size.png"), tr("&1024x768"), this);
    m_BgSize_1024_768->setCheckable(true);

    m_BgSize_1280_768 = new QAction(QIcon(":/icons/size.png"), tr("&1280x768"), this);
    m_BgSize_1280_768->setCheckable(true);

    backgroundSizeGroup->addAction(m_BgSizeDefault);
    backgroundSizeGroup->addAction(m_BgSize_A5_landscape);
    backgroundSizeGroup->addAction(m_BgSize_A5_portrait);
    backgroundSizeGroup->addAction(m_BgSize_A4_landscape);
    backgroundSizeGroup->addAction(m_BgSize_A4_portrait);
    backgroundSizeGroup->addAction(m_BgSize_800_600);
    backgroundSizeGroup->addAction(m_BgSize_1024_768);
    backgroundSizeGroup->addAction(m_BgSize_1280_768);
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
        automaticReloadIntervalActMap.insert(interval.first, intervalReloadAct );
        reloadIntervalGroup->addAction(intervalReloadAct);
    }

    createDesktopLinkAct = new QAction(QIcon(":/icons/create_link.svg"), tr("&Create desktop link"), this);
    createDesktopLinkAct->setStatusTip(tr("Create desktop link"));
    connect(createDesktopLinkAct, SIGNAL(triggered()), this, SLOT(createDesktopLinkSlot()));

    aboutAct = new QAction(QIcon(":/icons/about.svg"), tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(showAboutDialogSlot()));
}

void MainWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadProjectAct);
    fileMenu->addAction(loadProjectInNewInstanceAct);
    fileMenu->addAction(reloadProjectAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveProjectAct);
    fileMenu->addAction(backupProjectWithTimeStampAct);
    fileMenu->addAction(saveProjectAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(loadTextFileAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exportAsPictureAct);
    fileMenu->addAction(exportAsPdf);
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
    QMenu* defaultImagesMenu = backgroundMenu->addMenu(tr("&Default images"));
    defaultImagesMenu->addAction(m_BgDefaultImage1Action);
    defaultImagesMenu->addAction(m_BgDefaultImage2Action);
    QMenu* kanbanImagesMenu = backgroundMenu->addMenu(tr("&Kanban dashboards"));
    kanbanImagesMenu->addAction(m_BgImageKanban1Action);
    kanbanImagesMenu->addAction(m_BgImageKanban1HAction);
    kanbanImagesMenu->addAction(m_BgImageKanban2Action);
    kanbanImagesMenu->addAction(m_BgImageKanban2HAction);
    kanbanImagesMenu->addAction(m_BgImageKanban3Action);
    kanbanImagesMenu->addAction(m_BgImageKanban3HAction);
    QMenu* reviewImagesMenu = backgroundMenu->addMenu(tr("&Review dashboards"));
    reviewImagesMenu->addAction(m_BgImageReview1Action);
    reviewImagesMenu->addAction(m_BgImageReview2Action);
    backgroundMenu->addSeparator();
    backgroundMenu->addAction(m_BgUserImageAction);
    backgroundMenu->addSeparator();
    QMenu* sizeMenu = backgroundMenu->addMenu(QIcon(":/icons/sizes.svg"), tr("&Size"));
    sizeMenu->addAction(m_BgSizeDefault);
    sizeMenu->addAction(m_BgSize_A5_landscape);
    sizeMenu->addAction(m_BgSize_A5_portrait);
    sizeMenu->addAction(m_BgSize_A4_landscape);
    sizeMenu->addAction(m_BgSize_A4_portrait);
    sizeMenu->addAction(m_BgSize_800_600);
    sizeMenu->addAction(m_BgSize_1024_768);
    sizeMenu->addAction(m_BgSize_1280_768);
    backgroundMenu->addAction(m_BgFixedSize);

    menuBar()->addSeparator();
    QMenu* settingsMenu =  menuBar()->addMenu(tr("&Settings"));
    QMenu* automaticReloadMenu =settingsMenu->addMenu("Automatic project reload");
    foreach (QAction* action, automaticReloadIntervalActMap)
        automaticReloadMenu->addAction(action);
    settingsMenu->addSeparator();
    settingsMenu->addAction(createDesktopLinkAct);

    QMenu* helpMenu =  menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}
void MainWindow::loadProjectSlot()
{
    QString sFilename = QFileDialog::getOpenFileName(this, "Load project from file ", QString(),
                                                 tr("Semaphored project files (*.sem)"));
    if(sFilename.size())
       if(QMessageBox::question(this, tr("Load project"),
                                             tr("Do you want to <b>load project</b> %1 ? \nUnsaved changes in current project will be lost.").arg(sFilename),
                                             QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel) == QMessageBox::Yes)
            loadProject(sFilename);
}

void MainWindow::loadProjectInNewInstanceSlot()
{
    QString sFilename = QFileDialog::getOpenFileName(this, "Load project in new instance from file ", QString(),
                                                 tr("Semaphored project files (*.sem)"));
    if (sFilename.size())
        CommandLineArgs::getInstance()->startNewInstance(sFilename);
}

void MainWindow::loadProject(const QString& sFilename)
{
    if (!sFilename.startsWith(":/"))  {//files from resources
      m_sOpenedFile = sFilename;
      setWindowTitle(m_sOpenedFile);
      //TODO better logic, open file, new file
    }
    m_canvasWidget->loadProject(sFilename);
}

void MainWindow::reloadProjectSlot()
{
    if (m_sOpenedFile.size()) //we load only real files
        m_canvasWidget->loadProject(m_sOpenedFile);
}

void MainWindow::saveProjectSlot()
{
    if (!m_sOpenedFile.isEmpty())
        m_canvasWidget->saveProject(m_sOpenedFile);
    else
        saveProjectAsSlot();
}

void MainWindow::saveProjectAsSlot()
{
    QString sFilename = QFileDialog::getSaveFileName(this, "Save project as: ", "untitled.sem",
                                                     tr("Semaphored project files (*.sem)"));
    if(sFilename.size()) {
        QByteArray ext = QFileInfo(sFilename).suffix().toLower().toLatin1();
        if(ext != "sem")
            sFilename += ".sem";
        m_sOpenedFile = sFilename;
        setWindowTitle(m_sOpenedFile);
        m_canvasWidget->saveProject(m_sOpenedFile);
    }
}

void MainWindow::backupProjectWithTimeStampSlot()
{
    if(!m_sOpenedFile.isEmpty()) {
        QString sBackupFile = m_sOpenedFile;
        sBackupFile.chop(4); //
        sBackupFile += QDateTime::currentDateTime().toString("-yyyy_MM_dd-hh_mm");
        sBackupFile += ".sem";
        m_canvasWidget->saveProject(sBackupFile);
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
   //TPDP this could be unified by m_BackgroundPicture, "" - default, filename = picture, QColor::isValidColor  - color
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
    else if (action == m_BgImageKanban3Action)
        m_canvasWidget->changeBackgroundImage(DragWidget::BG_IMAGE_KANBAN_3);
   else if (action == m_BgImageKanban3HAction)
        m_canvasWidget->changeBackgroundImage(DragWidget::BG_IMAGE_KANBAN_3H);
    else if (action == m_BgImageReview1Action)
         m_canvasWidget->changeBackgroundImage(DragWidget::BG_IMAGE_REVIEW_1);
    else if (action == m_BgImageReview2Action)
         m_canvasWidget->changeBackgroundImage(DragWidget::BG_IMAGE_REVIEW_2);
    else if (action == m_BgUserImageAction) {
        m_canvasWidget->loadUserBackgroundImage();
   }
}

void  MainWindow::changeBackgroundFixed(bool checked )
{
    m_canvasWidget->setFixedSizeBg(checked);
}

void MainWindow::changeBackgroundSizeSlot(QAction * action)
{
    QSize newSize(m_canvasWidget->size());
    if (action == m_BgSizeDefault)
        ;
    else if (action == m_BgSize_A5_portrait)
        newSize = QSize(DragWidget::SIZE_A5_SHORT,DragWidget::SIZE_A5_LONG);
    else if (action == m_BgSize_A5_landscape)
        newSize = QSize(DragWidget::SIZE_A5_LONG,DragWidget::SIZE_A5_SHORT);
    else if (action == m_BgSize_A4_portrait)
        newSize = QSize(DragWidget::SIZE_A4_SHORT,DragWidget::SIZE_A4_LONG);
    else if (action == m_BgSize_A4_landscape)
        newSize = QSize(DragWidget::SIZE_A4_LONG,DragWidget::SIZE_A4_SHORT);
    else if (action == m_BgSize_800_600)
        newSize = QSize(800,600);
    else if (action == m_BgSize_1024_768)
        newSize = QSize(1024,768);
    else if (action == m_BgSize_1280_768)
        newSize = QSize(1280,768);
    resize(newSize);
}
void MainWindow::loadTextFileSlot()
{
    QString sFilename = QFileDialog::getOpenFileName(this, "Load source test file: ",QString(),
                                                     tr("Text files (*.txt *.*)"));
    if(sFilename.size())
        m_canvasWidget->loadTextFile(sFilename);
}

void MainWindow::changeIntervalReloadSlot(QAction * action)
{
    int i = automaticReloadIntervalActMap.key(action, 0);
    setIntervalReload(i);
}

void MainWindow::setIntervalReload(int interval)
{
   //todo New:1 - reload file in interval
}

DragWidget*  MainWindow::canvasWidget()
{
    return m_canvasWidget;
}

void MainWindow::printCurrentCanvasSlot()
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

void MainWindow::exportCanvasToPdfSlot()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export to PDF", QString(), "*.pdf");
    if (!fileName.isEmpty()) {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);

        QPixmap qpm = QPixmap::grabWidget(centralWidget());
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
    if (!m_aboutDialog) {
        m_aboutDialog = new AboutDialog(this);
    }
    m_aboutDialog->show();
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
