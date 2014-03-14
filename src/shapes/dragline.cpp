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

#include "dragline.h"
#include "dragwidget.h"
#include <QPainter>
#include <QtGlobal>
#include <QMenu>
#include <QContextMenuEvent>

DragLine::DragLine(const QPoint & p1, const QPoint & p2, DragWidget *canvasWidget) :
    QWidget(canvasWidget),
    AbstractDragInterface(canvasWidget),
    m_p1(p1),
    m_p2(p2),
    m_iLineWidth(LINE_WIDTH_NO_SELECTED),
    deleteAct(NULL)
{

    m_PaintingArea.setTop( qMin(p1.y(),p2.y()));
    m_PaintingArea.setBottom( qMax(p1.y(),p2.y()));
    m_PaintingArea.setLeft(qMin(p1.x(),p2.x()));
    m_PaintingArea.setRight(qMax(p1.x(),p2.x()));
    //relative to widget
    if(p1.x() <= p2.x()) {
        m_LineStart.setX(0);
        m_LineEnd.setX(p2.x() - p1.x());
    }
    else {
        m_LineStart.setX(p1.x() -p2.x());
        m_LineEnd.setX(0);
    }

    if(p1.y() <= p2.y()) {
        m_LineStart.setY(0);
        m_LineEnd.setY( p2.y()- p1.y());

    }
    else {
        m_LineStart.setY(p1.y() - p2.y());
        m_LineEnd.setY(0);
    }

    setAttribute( Qt::WA_TranslucentBackground, true );
    move(m_PaintingArea.left(), m_PaintingArea.top());
    resize(m_PaintingArea.size().width(), m_PaintingArea.size().height());

    deleteAct = new QAction(QIcon(":/icons/delete.svg"), tr("&Delete"), this);
    connect(deleteAct,SIGNAL(triggered()),this,SLOT(deleteItemSlot()));
}

void DragLine::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);
  QPen pen(Qt::black, m_iLineWidth, Qt::SolidLine);
  QPainter painter(this);
  painter.setPen(pen);
  painter.setBackgroundMode(Qt::TransparentMode);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);
  painter.drawLine(m_LineStart, m_LineEnd);
}
void DragLine::changeColor(const QColor &acolor)
{
    Q_UNUSED(acolor);
    //we don't want to change line colors for now
}

void DragLine::select(bool bSelected)
{
  if (bSelected)
    m_iLineWidth = LINE_WIDTH_SELECTED;
  else
    m_iLineWidth = LINE_WIDTH_NO_SELECTED;
  update();
}

QColor DragLine::currentColor()
{
    return palette().color(backgroundRole());
}

void DragLine::contextMenuEvent( QContextMenuEvent * event )
{
    event->accept();
    QMenu menu(this);
    menu.addAction(deleteAct);
    menu.exec(event->globalPos());
}

void DragLine::deleteItemSlot()
{
    if (m_CanvasWidget->isMultiselecting()) {
       m_CanvasWidget->deleteMutliselected();
    } else
      close();
}
