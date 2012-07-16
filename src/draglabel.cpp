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

#include <QContextMenuEvent>
#include <QMenu>
#include <QActionGroup>
#include "draglabel.h"
#include "yelloweditbox.h"

DragLabel::DragLabel(const QString &text, QWidget *parent, const QColor &defaultColor)
    : QLabel(text, parent),
      m_RightClickMenu(NULL)
{
    changeColor(defaultColor);
    setAutoFillBackground(true);
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
}

void DragLabel::changeColor(const QColor &acolor)
{
    if(currentColor() !=  acolor ) {
        QPalette pal = palette();
        pal.setColor(backgroundRole(), acolor);
        setPalette(pal);
        emit colorChangedSignal();
    }
}

void DragLabel::contextMenuEvent( QContextMenuEvent * event )
{
    event->accept();
    //if(!parent()->inherits("DragSquare")) { //separated
    const QAction* selectedAction = rightClickMenu()->exec(event->globalPos());
//    if (selectedAction == m_openAct) {
    //}
}

QMenu* DragLabel::rightClickMenu()     //context menu for tab switch
{
  if (!m_RightClickMenu)   {
    m_RightClickMenu = new QMenu(this);
    QMenu* colorMenu = new QMenu("Color", this);
    QActionGroup* colorGroup = new QActionGroup(this);
    pRedColorAction = new QAction(QIcon(":/images/red.svg"), tr("&Red"), this);
    pRedColorAction->setCheckable(true);
    if(currentColor()== Qt::red)
        pRedColorAction->setChecked(true);

    pOrangeColorAction = new QAction(QIcon(":/images/orange.svg"), tr("&Orange"), this);
    pOrangeColorAction->setCheckable(true);
    if(currentColor() == QColor(254,154,46))
        pOrangeColorAction->setChecked(true);

    pGreenColorAction = new QAction(QIcon(":/images/green.svg"), tr("&Green"), this);
    pGreenColorAction->setCheckable(true);
    if(currentColor()== Qt::green)
        pGreenColorAction->setChecked(true);

    pWhiteColorAction = new QAction(QIcon(":/images/white.svg"), tr("&White"), this);
    pWhiteColorAction->setCheckable(true);
    if(currentColor()== Qt::white)
        pWhiteColorAction->setChecked(true);

    pGrayColorAction = new QAction(QIcon(":/images/gray.svg"), tr("&Gray"), this);
    pGrayColorAction->setCheckable(true);
    if(currentColor()== Qt::lightGray)
        pGrayColorAction->setChecked(true);

    //group
    colorGroup->addAction(pRedColorAction);
    colorGroup->addAction(pOrangeColorAction);
    colorGroup->addAction(pGreenColorAction);
    colorGroup->addAction(pWhiteColorAction);
    colorGroup->addAction(pGrayColorAction);
    colorGroup->setExclusive(true);
    connect(colorGroup,SIGNAL(triggered(QAction *)),this,SLOT(changeColorSlot(QAction*)));

    //menu
    colorMenu->addAction(pRedColorAction);
    colorMenu->addAction(pOrangeColorAction);
    colorMenu->addAction(pGreenColorAction);
    colorMenu->addSeparator();
    colorMenu->addAction(pWhiteColorAction);
    colorMenu->addAction(pGrayColorAction);

    QAction* pDeleteAction = new QAction(QIcon(":/images/delete.svg"), tr("&Delete"), this);
    connect(pDeleteAction,SIGNAL(triggered()),this,SLOT(deleteItemSlot()));


    m_RightClickMenu->addMenu(colorMenu);
    m_RightClickMenu->addAction(pDeleteAction);
  }
  return m_RightClickMenu;
}

void DragLabel::changeColorSlot(QAction* action)
{
    QColor newColor(Qt::white);
    if (action == pRedColorAction )
        newColor = Qt::red;
    else if (action == pOrangeColorAction)
        newColor = QColor(254,154,46);//newColor = QColor(255,128,64);
    else if (action == pGreenColorAction)
        newColor = Qt::green;
    else if (action == pWhiteColorAction)
        newColor = Qt::white;
    else if (action == pGrayColorAction)
        newColor = Qt::lightGray;

    changeColor(newColor);
}

void DragLabel::mouseDoubleClickEvent ( QMouseEvent * event )
{
    emit editSlot();
}

void DragLabel::editSlot()
{
    YellowEditBox* captionEdit = new YellowEditBox(this);
    //captionEdit->installEventFilter(captionEdit);
    captionEdit->show();
    captionEdit->setFocus();
}

QColor DragLabel::currentColor()
{
    return palette().color(backgroundRole());
}

void DragLabel::deleteItemSlot()
{
    if(parentWidget()->inherits("DragSquare"))
       parentWidget()->close();
    else
      close();
}
