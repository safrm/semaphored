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

#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>
QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QMenu;
class QAction;
class DragLabel;


QT_END_NAMESPACE

class DragWidget : public QWidget
{
    Q_OBJECT
public:
    DragWidget(QWidget *parent = 0);
    enum { WHITE =  0,
           RED   =  1,
           ORANGE = 2,
           GREEN  = 3} color;
public slots:
    void deleteAllItemsSlot();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent (QMouseEvent * event);

    virtual void contextMenuEvent ( QContextMenuEvent * event );

private:
    QMenu* rightClickMenu();
    QMenu* m_RightClickMenu;

    QAction* m_NewLabelAction;
    QAction* m_NewSquareAction;

    QList<const DragLabel*> selectedItems;
    QPoint selectionStart;
    bool bSelecting;
};

#endif
