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
#include "draglabel.h"
#include "dragwidget.h"
#include "dragsquare.h"
#include "yelloweditbox.h"

DragWidget::DragWidget(QWidget *parent)
    : QWidget(parent),
      m_RightClickMenu(NULL),
     m_NewLabelAction(NULL),
     m_NewSquareAction(NULL),
     selectedItems(),
     selectionStart(),
     bSelecting(false)
{
    QFile dictionaryFile(":/dictionary/words.txt");
    dictionaryFile.open(QIODevice::ReadOnly);
    QTextStream inputStream(&dictionaryFile);

    int x = 5;
    int y = 5;
/*  word by word
    while (!inputStream.atEnd()) {
        QString word;
        inputStream >> word;
        if (!word.isEmpty()) {
            DragLabel *wordLabel = new DragLabel(word, this);
            wordLabel->move(x, y);
            wordLabel->show();
            wordLabel->setAttribute(Qt::WA_DeleteOnClose);
            x += wordLabel->width() + 2;
            if (x >= 195) {
                x = 5;
                y += wordLabel->height() + 2;
            }
        }
    }
    */
    //line by  line
    QString line;
    do {
        line = inputStream.readLine();
        if (!line.isEmpty()) {
            DragLabel *wordLabel = new DragLabel(line, this);
            wordLabel->move(x, y);
            wordLabel->show();
            wordLabel->setAttribute(Qt::WA_DeleteOnClose);
            y += wordLabel->height() + 2;
            if (y + 10 >= height()) {
                y = 5;
                x += 150 ;
            }
        }
    } while (!line.isNull());

    //add few testing squares
    DragSquare *wordSqare = new DragSquare("test","content", this);
    wordSqare->move(50, 50);
    wordSqare->show();
    wordSqare->setAttribute(Qt::WA_DeleteOnClose);

    //backround color?
    QPalette newPalette = palette();
    newPalette.setColor(QPalette::Window, Qt::white);
    setPalette(newPalette);

    setAcceptDrops(true);
    setMinimumSize(600, qMax(300, y));
    setWindowTitle(tr("sempathored"));
}

void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void DragWidget::dropEvent(QDropEvent *event)
{
   /* QString sCurrentObjName = event->mimeData()->objectName();
    if (sCurrentObjName == "DragLabel") {
    }
    */
    if (event->mimeData()->hasText()) {
        const QMimeData *mime = event->mimeData();
        QPoint position = event->pos();
        QPoint hotSpot;
        QColor color(Qt::white);


        QList<QByteArray> hotSpotPos = mime->data("application/x-hotspot").split(' ');
        if (hotSpotPos.size() == 2) {
            hotSpot.setX(hotSpotPos.first().toInt());
            hotSpot.setY(hotSpotPos.last().toInt());
        }
        if(mime->hasColor()) {
            color = qvariant_cast<QColor>(mime->colorData());
        }

        QString label = mime->text();

        //square for now
        if(mime->hasHtml()) {
            QString text = mime->html();
            DragSquare *newSquare = new DragSquare(label, text, this);
            newSquare->move(position - hotSpot);
            newSquare->changeColor(color);
            newSquare->show();
            newSquare->setAttribute(Qt::WA_DeleteOnClose);
            position += QPoint(newSquare->width(), 0);
        } else { //label
          DragLabel *newLabel = new DragLabel(label, this);
          newLabel->move(position - hotSpot);
          newLabel->changeColor(color);
          newLabel->show();
          newLabel->setAttribute(Qt::WA_DeleteOnClose);
          position += QPoint(newLabel->width(), 0);
        }


        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
    foreach (QObject *child, children()) {
        if (child->inherits("QWidget")) {
            QWidget *widget = static_cast<QWidget *>(child);
            if (!widget->isVisible())
               widget->deleteLater();
        }
    }
}

void DragWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget * widget = childAt(event->pos());
    //we pressed out of our objects
    if (!widget) {
        //cancel YellowBox edit
        //a bit stupid way but easy.. maybe it could restoe old text instead of applying new one?
        foreach (QObject *yellowBox, children()) {
            if (yellowBox->inherits("YellowEditBox")) {
                emit (static_cast<YellowEditBox*>(yellowBox))->updateText();
                return;
            }
        }
        //we have selection so we want to clean it
        if(selectedItems.size()) {
           selectedItems.clear();
           return;
        } else {
        //we don't have selection so we try to create it
        selectionStart = event->pos();
        bSelecting = true;
        return;
        }
    }

    if (event->button() == Qt::RightButton)
        return;

    //TODO we have selection and want to work with it
    if(selectedItems.size()) {
      //QList<QPoint> hotSpots;
      //QMimeData *mimeData = new QMimeData;
      return;
    }

    DragLabel *labelChild(NULL);
    DragSquare *squareChild(NULL);

    //1 label
    if (widget->inherits("DragLabel"))
        labelChild = static_cast<DragLabel*>(widget);
    else if (widget->inherits("DragSquare"))
        squareChild = static_cast<DragSquare*>(widget);

    if(labelChild) {
        if(widget->parent()->inherits("DragSquare")) {
            squareChild = static_cast<DragSquare*>(labelChild->parent()); //we take the whole square
        } else {
          QPoint hotSpot = event->pos() - labelChild->pos();

            QMimeData *mimeData = new QMimeData;
            mimeData->setText(labelChild->text());
            mimeData->setData("application/x-hotspot",
                              QByteArray::number(hotSpot.x())
                              + " " + QByteArray::number(hotSpot.y()));
            mimeData->setColorData(labelChild->currentColor());


            QPixmap pixmap(labelChild->size());
            labelChild->render(&pixmap);

            QDrag *drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap);
            drag->setHotSpot(hotSpot);

            Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

            if (dropAction == Qt::MoveAction)
                labelChild->close();
        }
    }
    //1 composed square
    if (squareChild) {
        QPoint hotSpot = event->pos() - squareChild->pos();

         QMimeData *mimeData = new QMimeData;
         mimeData->setText(squareChild->label());
         mimeData->setHtml(squareChild->text());
         mimeData->setData("application/x-hotspot",
                           QByteArray::number(hotSpot.x())
                           + " " + QByteArray::number(hotSpot.y()));
         mimeData->setColorData(squareChild->currentColor());


         QPixmap pixmap(squareChild->size());
         squareChild->render(&pixmap);

         QDrag *drag = new QDrag(this);
         drag->setMimeData(mimeData);
         drag->setPixmap(pixmap);
         drag->setHotSpot(hotSpot);

         Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

         if (dropAction == Qt::MoveAction)
             squareChild->close();
    }

}

void DragWidget::mouseReleaseEvent (QMouseEvent * event)
{
    if(bSelecting) {
        bSelecting = false;
        QPoint selectionEnd = event->pos();
        QRect selectionRect(selectionStart,selectionEnd);
        foreach (QObject *child, children()) {
            if (child->inherits("DragLabel")) {
                const DragLabel *widget = static_cast<const DragLabel *>(child);
                if(selectionRect.contains(widget->geometry()))
                    selectedItems += widget;
            }
        }
    }
}

void  DragWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    event->accept();
    QPoint pos = event->globalPos();
    QAction* selectedAction = rightClickMenu()->exec(pos);
    if (selectedAction == m_NewLabelAction) {
        DragLabel *wordLabel = new DragLabel("line", this);
        wordLabel->move(event->pos());
        wordLabel->show();
        wordLabel->setAttribute(Qt::WA_DeleteOnClose);
        wordLabel->editSlot();
    } else if (selectedAction == m_NewSquareAction) {
        DragSquare *wordSqare = new DragSquare("newtest","content2", this);
        wordSqare->move(event->pos());
        wordSqare->show();
        wordSqare->setAttribute(Qt::WA_DeleteOnClose);
        wordSqare->editLabelSlot();
     }

}

QMenu* DragWidget::rightClickMenu()
{
  if (!m_RightClickMenu) {
    m_RightClickMenu = new QMenu(this);
    QMenu* newMenu = new QMenu("New", this);

    m_NewLabelAction = new QAction(QIcon(":/images/new_label.svg"), tr("&Label"), this);
    newMenu->addAction(m_NewLabelAction);

    m_NewSquareAction = new QAction(QIcon(":/images/new_square.svg"), tr("&Square"), this);
    newMenu->addAction(m_NewSquareAction);

    m_RightClickMenu->addMenu(newMenu);
  }
  return m_RightClickMenu;
}

void DragWidget::deleteAllItemsSlot()
{
    foreach (QObject *child, children()) {
        if (child->inherits("DragLabel")) {
            DragLabel *widget = static_cast<DragLabel *>(child);
            widget->deleteLater();
        }
        if (child->inherits("DragSquare")) {
            DragSquare *widget = static_cast<DragSquare *>(child);
            widget->deleteLater();
        }
    }
}
