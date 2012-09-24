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

#ifndef DRAGLABEL_H
#define DRAGLABEL_H

#include <QLabel>
#include <qnamespace.h>
#include "abstractdraginterface.h"

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDragMoveEvent;
class QFrame;
class QMenu;
class QLineEdit;
class DragWidget;
QT_END_NAMESPACE

class DragLabel : public QLabel, public AbstractDragInterface
{
    Q_OBJECT
    Q_INTERFACES(AbstractDragInterface)

public:
    DragLabel(const QString &text, QWidget* parent, DragWidget* canvasWidget, const QColor &defaultColor = Qt::white);
    QMenu* rightClickMenu();

    //abstract behaviour
    virtual QColor currentColor();
    virtual void changeColor(const QColor &acolor);
    virtual void select(bool bSelected);

signals:
    void colorChangedSignal();
public slots:
    void editSlot();

private slots:
    void changeColorSlot(QAction *action);
    void deleteItemSlot();

protected :
    virtual void contextMenuEvent ( QContextMenuEvent * event );
    virtual void mouseDoubleClickEvent ( QMouseEvent * event );


private:
    QMenu*   m_RightClickMenu;
    QAction* pWhiteColorAction;
    QAction* pGrayColorAction;
    QAction* pGreenColorAction;
    QAction* pOrangeColorAction;
    QAction* pRedColorAction;
    DragWidget* m_CanvasWidget;
};

#endif
