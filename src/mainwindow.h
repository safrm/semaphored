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

signals:
    
public slots:
    void loadProjectSlot();
    void saveProjectSlot();
    void backupProjectWithTimeStampSlot();
    void saveProjectAsSlot();
    void exportAsPictureSlot();
    void changeBackgroundColorSlot(QAction* action);
    void loadTextFileSlot();
    void printCurrentCanvasSlot();
    void exportCanvasToPdfSlot();
    void showAboutDialogSlot();

private:
    void createActions();
    void createMenus();
//    void createToolBars();
//    void createStatusBar();
//    void createDockWindows();

    QAction* loadProjectAct;
    QAction* saveProjectAct;
    QAction* backupProjectWithTimeStampAct;
    QAction* saveProjectAsAct;
    QAction* exportAsPictureAct;
    QAction* loadTextFileAct;
    QAction* printAct;
    QAction* exportAsPdf;
    QAction* quitAct;
    QAction* aboutAct;
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
    QAction* m_BgUserImageAction;
    DragWidget* m_canvasWidget;
    AboutDialog* m_aboutDialog;
    QString m_sOpenedFile;
};

#endif // MAINWINDOW_H
