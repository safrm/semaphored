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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMap>

class QAction;
class DragWidget;
class QPrinter;
class AboutDialog;
class QTimer;
class MainWindow_private;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    DragWidget* canvasWidget();
    static MainWindow *instance();
    void loadProject(const QString & sFilename);
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
    void changeBackgroundSizeSlot(QAction* action);
    void changeIntervalReloadSlot(QAction* action);
    void intervalReloadSlot();
    void loadTextFileSlot();
    void exportToTextFileSlot();
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

    MainWindow_private* d;
};

#endif // MAINWINDOW_H
