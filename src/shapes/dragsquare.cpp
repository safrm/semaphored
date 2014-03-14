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

#include <QVBoxLayout>
#include <QTextEdit>
#include <QMenu>
#include <QContextMenuEvent>

#include "dragsquare.h"
#include "draglabel.h"
#include "dragwidget.h"

DragSquare::DragSquare(const QString &label, const QString &text, DragWidget* canvasWidget, const QColor &defaultColor )
    : QFrame(canvasWidget),
      AbstractDragInterface(canvasWidget)
{
    setAutoFillBackground(true);
    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Raised);

    QVBoxLayout * mainLayout= new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    labelDragLabel = new DragLabel(label, this, canvasWidget, defaultColor);
    connect( labelDragLabel, SIGNAL(colorChangedSignal()), this, SLOT(updateColorsSlot()));
    textTextEdit = new QTextEdit(text, this);
    //int iHeight = textTextEdit->fontPointSize();
    //int iWidth = labelDragLabel->width();
    textTextEdit->setAutoFillBackground(true);
    textTextEdit->setFrameShape(QFrame::NoFrame);
    labelDragLabel->setFrameShape(QFrame::NoFrame);
    labelDragLabel->setIndent(4);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);
    textTextEdit->setMaximumSize(labelDragLabel->width(),60);
    mainLayout->addWidget(labelDragLabel);
    QFrame * separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setLineWidth(2);
    mainLayout->addWidget(separator);
    mainLayout->addWidget(textTextEdit);
    setLayout(mainLayout);
    updateColorsSlot();
}

void DragSquare::contextMenuEvent( QContextMenuEvent * event )
{
    event->accept();
    labelDragLabel->rightClickMenu()->exec(event->globalPos());
    //const QAction* selectedAction = rightClickMenu()->exec(event->globalPos());
}

//stupid way!!
void DragSquare::changeColor(const QColor &acolor)
{
    labelDragLabel->changeColor(acolor);
}

void DragSquare::updateColorsSlot()
{
    QPalette pal = palette();
    pal.setColor(backgroundRole(), labelDragLabel->currentColor());
    pal.setColor(QPalette::Base,labelDragLabel->currentColor());
    setPalette(pal);

    textTextEdit->setPalette(pal);
}

void DragSquare::editLabelSlot()
{
    labelDragLabel->editSlot();
}
QString DragSquare::label()
{
    return labelDragLabel->text();
}

QString DragSquare::text()
{
    return textTextEdit->toPlainText();
}

QColor DragSquare::currentColor()
{
    return labelDragLabel->currentColor();
}

DragLabel* DragSquare::labelWidget()
{
    return labelDragLabel;
}

void DragSquare::select(bool bSelected)
{
   if(bSelected)
        setLineWidth(4);
    else
        setLineWidth(2);
}


