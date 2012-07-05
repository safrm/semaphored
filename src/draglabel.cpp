/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
# include <QActionGroup>
#include "draglabel.h"
#include "yelloweditbox.h"

DragLabel::DragLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent),
      m_RightClickMenu(NULL),
      currentColor(Qt::white)
{
    changeColor(Qt::white);
    setAutoFillBackground(true);
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
}

void DragLabel::changeColor(const QColor &acolor)
{
    currentColor = acolor;
    QPalette pal = palette();
    pal.setColor(backgroundRole(), acolor);
    setPalette(pal);
}

void DragLabel::contextMenuEvent( QContextMenuEvent * event )
{
    //first ativate right editor window
    /*QPoint position = event->pos();
    int c = tabBar()->count();
    int clickedItem(-1);
    int i(0);
    for (; i<c; ++i) {
    if (tabBar()->tabRect(i).contains(position)) {
      clickedItem = i;
      break;
    }
    }
    if (clickedItem != -1)
    setCurrentIndex(clickedItem);
*/
    //execute menu
    const QAction* selectedAction = rightClickMenu()->exec(event->globalPos());
//    if (selectedAction == m_openAct) {
}
QMenu* DragLabel::rightClickMenu()     //context menu for tab switch
{
  if (!m_RightClickMenu)
  {
    m_RightClickMenu = new QMenu(this);
    QMenu* colorMenu = new QMenu("Color", this);
    QActionGroup* colorGroup = new QActionGroup(this);
    QAction* pRedColorAction = new QAction(QIcon(":/images/red.svg"), tr("&Red"), this);
    connect(pRedColorAction,SIGNAL(triggered()),this,SLOT(changeColor(Qt::red)));
    pRedColorAction->setCheckable(true);
    colorMenu->addAction(pRedColorAction);

    QAction* pOrangeColorAction = new QAction(QIcon(":/images/orange.svg"), tr("&Orange"), this);
    //connect(pCopyFullFileNameAction,SIGNAL(triggered()),this,SLOT(copyFullFileName()));
    pOrangeColorAction->setCheckable(true);
    colorMenu->addAction(pOrangeColorAction);

    QAction* pGreenColorAction = new QAction(QIcon(":/images/green.svg"), tr("&Green"), this);
    //connect(pCopyFullFileNameAction,SIGNAL(triggered()),this,SLOT(copyFullFileName()));
    pGreenColorAction->setCheckable(true);
    colorMenu->addAction(pGreenColorAction);
    colorMenu->addSeparator();
    QAction* pWhiteColorAction = new QAction(QIcon(":/images/white.svg"), tr("&White"), this);
    //connect(pCopyFullFileNameAction,SIGNAL(triggered()),this,SLOT(copyFullFileName()));
    pWhiteColorAction->setCheckable(true);
    colorMenu->addAction(pWhiteColorAction);
    colorGroup->setExclusive(true);

    colorMenu->addActions(colorGroup->actions());
    m_RightClickMenu->addMenu(colorMenu);

  }

  return m_RightClickMenu;
}

void DragLabel::mouseDoubleClickEvent ( QMouseEvent * event )
{
    //edit
    YellowEditBox* captionEdit = new YellowEditBox(this);

    captionEdit->show();
    captionEdit->setFocus();
}


