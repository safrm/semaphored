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
#include <QDomElement>


DragLine::DragLine(const QPoint & p1, const QPoint & p2, DragWidget *canvasWidget) :
    DragBaseLine(p1,p2,canvasWidget),
    m_LineEnding_p1(Plain),
    m_LineEnding_p2(Plain),
    m_RightClickMenu(NULL)
{
}

void DragLine::contextMenuEvent( QContextMenuEvent * event )
{
    event->accept();
    const QAction* selectedAction = rightClickMenu()->exec(event->globalPos());
    Q_UNUSED(selectedAction);
}

QMenu* DragLine::rightClickMenu()
{
    if (!m_RightClickMenu)   {
      m_RightClickMenu = new QMenu(this);
      QMenu* pWidthMenu = new QMenu("Width", this);
        pWidthMenu->setIcon(QIcon(":/icons/color.svg"));
      QActionGroup* widthGroup = new QActionGroup(this);
      m_p2pAction = new QAction(QIcon(":/icons/line-width-2.svg"), tr("&2p"), this);
      m_p2pAction->setCheckable(true);
      if(m_iLineWidth == LINE_WIDTH_2)
          m_p2pAction->setChecked(true);

      m_p4pAction = new QAction(QIcon(":/icons/line-width-4.svg"), tr("&4p"), this);
      m_p4pAction->setCheckable(true);
      if(m_iLineWidth == LINE_WIDTH_4)
          m_p4pAction->setChecked(true);

      m_p8pAction = new QAction(QIcon(":/icons/line-width-8.svg"), tr("&8p"), this);
      m_p8pAction->setCheckable(true);
      if(m_iLineWidth == LINE_WIDTH_8)
          m_p8pAction->setChecked(true);

      //group
      widthGroup->addAction(m_p2pAction);
      widthGroup->addAction(m_p4pAction);
      widthGroup->addAction(m_p8pAction);
      widthGroup->setExclusive(true);
      connect(widthGroup,SIGNAL(triggered(QAction *)),this,SLOT(changeWidthSlot(QAction*)));

      //menu
      pWidthMenu->addAction(m_p2pAction);
      pWidthMenu->addAction(m_p4pAction);
      pWidthMenu->addAction(m_p8pAction);


      QAction* deleteAct = new QAction(QIcon(":/icons/delete.svg"), tr("&Delete"), this);
      connect(deleteAct,SIGNAL(triggered()),this,SLOT(deleteItemSlot()));

      m_RightClickMenu->addMenu(pWidthMenu);
      //m_RightClickMenu->addMenu(pStyleMenu);
      //m_RightClickMenu->addMenu(pLeftEndingMenu);
      //m_RightClickMenu->addMenu(pRightEndingMenu);
      m_RightClickMenu->addAction(deleteAct);
    }
    return m_RightClickMenu;

}

void DragLine::changeWidthSlot(QAction* action)
{
    if (action==m_p2pAction) {
        m_iLineWidth=LINE_WIDTH_2;
        m_iLineWidthToPaint=LINE_WIDTH_2;
    }
    else if (action==m_p4pAction) {
        m_iLineWidth=LINE_WIDTH_4;
        m_iLineWidthToPaint=LINE_WIDTH_4;
    }
    else if (action==m_p8pAction) {
     m_iLineWidth=LINE_WIDTH_8;
     m_iLineWidthToPaint=LINE_WIDTH_8;
    }
    else
        ;

    update();

}

void DragLine::deleteItemSlot()
{
    if (m_CanvasWidget->isMultiselecting()) {
       m_CanvasWidget->deleteMutliselected();
    } else
      close();
}

void DragLine::fillXmlElement(QDomElement & element)
{
}
