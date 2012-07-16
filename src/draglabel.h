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

#ifndef DRAGLABEL_H
#define DRAGLABEL_H

#include <QLabel>
#include <qnamespace.h>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDragMoveEvent;
class QFrame;
class QMenu;
class QLineEdit;
QT_END_NAMESPACE

class DragLabel : public QLabel
{
    Q_OBJECT
public:
    DragLabel(const QString &text, QWidget *parent, const QColor &defaultColor = Qt::white);
    void changeColor(const QColor &acolor);
    QColor currentColor();

signals:
    void colorChangedSignal();
public slots:
    void editSlot();

public:
    QMenu* rightClickMenu();
private slots:
    void changeColorSlot(QAction *action);
    void deleteItemSlot();

protected :
    virtual void contextMenuEvent ( QContextMenuEvent * event );
    virtual void mouseDoubleClickEvent ( QMouseEvent * event );


private:
    //QColor m_currentColor;
    QMenu*   m_RightClickMenu;
    QAction* pWhiteColorAction;
    QAction* pGrayColorAction;
    QAction* pGreenColorAction;
    QAction* pOrangeColorAction;
    QAction* pRedColorAction;
};

#endif
