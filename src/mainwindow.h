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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMap>

class QAction;
class DragWidget;
class QPrinter;
class AboutDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    DragWidget* canvasWidget();
    static MainWindow *instance();
    void loadProject(const QString & sFilename);
    void setIntervalReload(int insterval);
signals:
    
public slots:
    void loadProjectSlot();
    void loadProjectInNewInstanceSlot();
    void reloadProjectSlot();
    void saveProjectSlot();
    void backupProjectWithTimeStampSlot();
    void saveProjectAsSlot();
    void exportAsPictureSlot();
    void changeBackgroundColorSlot(QAction* action);
    void changeBackgroundFixed(bool checked );
    void changeBackgroundSizeSlot(QAction* action);
    void changeIntervalReloadSlot(QAction* action);
    void loadTextFileSlot();
    void printCurrentCanvasSlot();
    void exportCanvasToPdfSlot();
    void showAboutDialogSlot();
    void createDesktopLinkSlot();    

protected slots:
    void slotPlaceToTray();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

signals:
  void signalPlaceToTray();

protected:
  virtual void changeEvent(QEvent *ev);

private:
    void createActions();
    void createMenus();
//    void createToolBars();
//    void createStatusBar();
//    void createDockWindows();

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

};

#endif // MAINWINDOW_H
