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

#include <QContextMenuEvent>
#include <QMenu>
#include <QActionGroup>
#include <QDomElement>

#include "draglabel.h"
#include "yelloweditbox.h"
#include "dragwidget.h"
#include "dragsquare.h"

DragLabel::DragLabel(const QString &text, QWidget *parent, DragWidget * canvasWidget, const QColor &defaultColor)
    : QLabel(text, parent),
      AbstractDragInterface(canvasWidget),
      m_RightClickMenu(NULL)
{
    setAttribute(Qt::WA_DeleteOnClose);
    changeColor(defaultColor);
    setAutoFillBackground(true);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);
    show();
}

DragLabel::DragLabel(const QString &text, QWidget* parent, DragWidget* canvasWidget, const QColor &defaultColor, qint64 timeStamp, int x, int y)
    : QLabel(text, parent),
      AbstractDragInterface(canvasWidget),
      m_RightClickMenu(NULL)
{
    setAttribute(Qt::WA_DeleteOnClose);
    changeColor(defaultColor);
    setAutoFillBackground(true);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);

    setTimeStamp(timeStamp);
    move(x,y);
    show();
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
    Q_UNUSED(selectedAction);
//    if (selectedAction == m_openAct) {
    //}
}

QMenu* DragLabel::rightClickMenu()
{
  if (!m_RightClickMenu) {
    m_RightClickMenu = new QMenu(this);
    QMenu* colorMenu = new QMenu("Color", this);
    colorMenu->setIcon(QIcon(":/icons/color.svg"));
    QActionGroup* colorGroup = new QActionGroup(this);
    pRedColorAction = new QAction(QIcon(":/icons/red.svg"), tr("&Red"), this);
    pRedColorAction->setCheckable(true);
    if(currentColor()== Qt::red)
        pRedColorAction->setChecked(true);

    pOrangeColorAction = new QAction(QIcon(":/icons/orange.svg"), tr("&Orange"), this);
    pOrangeColorAction->setCheckable(true);
    if(currentColor() == QColor(254,154,46))
        pOrangeColorAction->setChecked(true);

    pGreenColorAction = new QAction(QIcon(":/icons/green.svg"), tr("&Green"), this);
    pGreenColorAction->setCheckable(true);
    if(currentColor()== Qt::green)
        pGreenColorAction->setChecked(true);

    pWhiteColorAction = new QAction(QIcon(":/icons/white.svg"), tr("&White"), this);
    pWhiteColorAction->setCheckable(true);
    if(currentColor()== Qt::white)
        pWhiteColorAction->setChecked(true);

    pGrayColorAction = new QAction(QIcon(":/icons/gray.svg"), tr("&Gray"), this);
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

    QAction* pDeleteAction = new QAction(QIcon(":/icons/delete.svg"), tr("&Delete"), this);
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

   if (m_CanvasWidget->isMultiselecting()) {
      m_CanvasWidget->changeColorMutliselected(newColor);
   } else {
       changeColor(newColor);
   }
}

void DragLabel::mouseDoubleClickEvent ( QMouseEvent * event )
{
    Q_UNUSED(event);
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
    if (m_CanvasWidget->isMultiselecting()) {
       m_CanvasWidget->deleteMutliselected();
    }
    else if(parentWidget()->inherits("DragSquare"))
       parentWidget()->close();
    else
      close();
}

void DragLabel::select(bool bSelected)
{
    if(parentWidget()->inherits("DragSquare"))
        qobject_cast<DragSquare*>(parentWidget())->select(bSelected); //qobject_cast<DragSquare*>(parentWidget())->setLineWidth(iLineWidth);
    else {
        if(bSelected)
            setLineWidth(BORDER_WIDTH_SELECTED);
        else
            setLineWidth(BORDER_WIDTH_NO_SELECTED);
    }
}

void DragLabel::fillXmlElement(QDomElement & element)
{
    element.setAttribute("created", QString::number(creationTimeStamp()));
    element.setAttribute("color", currentColor().name());
    element.setAttribute("label", text());
    element.setAttribute("x", QString::number(pos().x()));
    element.setAttribute("y", QString::number(pos().y()));
}
