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

#include <QtGui>
#include <QActionGroup>
#include <QRubberBand>
#include <QDomDocument>
#include <QTextStream>
#include <QFile>
#include <QtAlgorithms>
#include <QSvgGenerator>

#include <unistd.h>

#include "draglabel.h"
#include "dragwidget.h"
#include "dragsquare.h"
#include "dragline.h"
#include "dragbaseline.h"
#include "yelloweditbox.h"
#include "version.h"
#include "abstractdraginterface.h"

QString DragWidget::BG_IMAGE_DEFAULT_1(":/images/default.png");
QString DragWidget::BG_IMAGE_DEFAULT_2(":/images/bg-semaphored-a5.png");
QString DragWidget::BG_IMAGE_KANBAN_1(":/images/bg-kanban1-a5.png");
QString DragWidget::BG_IMAGE_KANBAN_1H(":/images/bg-kanban1-a5h.png");
QString DragWidget::BG_IMAGE_KANBAN_2(":/images/bg-kanban2-a5.png");
QString DragWidget::BG_IMAGE_KANBAN_2H(":/images/bg-kanban2-a5h.png");
QString DragWidget::BG_IMAGE_KANBAN_3(":/images/bg-kanban3-a5.png");
QString DragWidget::BG_IMAGE_KANBAN_3H(":/images/bg-kanban3-a5h.png");
QString DragWidget::BG_IMAGE_REVIEW_1(":/images/bg-review1-a5.png");
QString DragWidget::BG_IMAGE_REVIEW_2(":/images/bg-review2-a5.png");
QSize DragWidget::DEFAULT_SIZE(720,445); //A5
QSize DragWidget::MINIMUM_SIZE(100,100);
int DragWidget::SIZE_A5_SHORT(445);
int DragWidget::SIZE_A5_LONG(720);
int DragWidget::SIZE_A4_SHORT(720);
int DragWidget::SIZE_A4_LONG(890);
QMenu * DragWidget::m_RightClickMenu(NULL);


DragWidget::DragWidget(QWidget *parent)
    : QWidget(parent),
      m_NewLabelAction(NULL),
      m_NewSquareAction(NULL),
      m_NewLineAction(NULL),
      m_NewBaseLineAction(NULL),
      selectedItems(),
      m_selectionStartPoint(),
      m_selectionEndPoint(),
      multiselectRubberBand(NULL),
      m_bPaintLine(false),
      m_bBaseLine(false),
      m_BackgroundPicture(""),
      m_bFixedBgSize(false)
{

}

void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{


}

void DragWidget::dropEvent(QDropEvent *event)
{

}

void DragWidget::mousePressEvent(QMouseEvent *event)
{


}
void DragWidget::mouseMoveEvent(QMouseEvent *event)
{

}

void DragWidget::mouseReleaseEvent(QMouseEvent * event)
{

}

void DragWidget::contextMenuEvent ( QContextMenuEvent * event )
{

}

void DragWidget::paintEvent(QPaintEvent *event)
{

}

void DragWidget::resizeEvent ( QResizeEvent * event )
{
}

void DragWidget::setFixedSizeBg(bool bFixed)
{
}

void DragWidget::deleteAllItemsSlot()
{
}
