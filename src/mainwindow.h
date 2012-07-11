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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QAction;
class DragWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    
signals:
    
public slots:
    void exportAsPictureSlot();
    void changeBackgroundColorSlot(QAction* action);

private:
    void createActions();
    void createMenus();
//    void createToolBars();
//    void createStatusBar();
//    void createDockWindows();

    QAction* exportAsPictureAct;
    QAction* deleteAllAct;
    QAction* loadBackgroundImageAct;
    QAction* m_BgColorWhiteAction;
    QAction* m_BgColorGrayAction ;
    QAction* m_BgColorCyanAction;
    DragWidget* m_canvasWidget;
};

#endif // MAINWINDOW_H
