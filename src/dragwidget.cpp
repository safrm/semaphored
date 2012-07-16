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

#include <QtGui>
#include <QActionGroup>

#include "draglabel.h"
#include "dragwidget.h"
#include "dragsquare.h"
#include "yelloweditbox.h"

QString DragWidget::BG_IMAGE_DEFAULT_1(":/images/default.png");
QString DragWidget::BG_IMAGE_DEFAULT_2(":/images/bg-semaphored-a5.png");
QString DragWidget::BG_IMAGE_KANBAN_1(":/images/bg-kanban1-a5.png");
QString DragWidget::BG_IMAGE_KANBAN_1H(":/images/bg-kanban1-a5h.png");
QString DragWidget::BG_IMAGE_KANBAN_2(":/images/bg-kanban2-a5.png");
QString DragWidget::BG_IMAGE_KANBAN_2H(":/images/bg-kanban2-a5h.png");
QSize DragWidget::DEFAULT_SIZE(720,445); //A5
QMenu * DragWidget::m_RightClickMenu(NULL);

DragWidget::DragWidget(QWidget *parent)
    : QWidget(parent),
     m_NewLabelAction(NULL),
     m_NewSquareAction(NULL),
     selectedItems(),
     selectionStart(),
     bSelecting(false)
{

    loadTextFile(QString(":/dictionary/words.txt"), true);

    //add few testing squares
    DragSquare *wordSqare = new DragSquare("test","content", this);
    wordSqare->move(250, 50);
    wordSqare->show();
    wordSqare->setAttribute(Qt::WA_DeleteOnClose);

    //backround color
    QPalette pal = palette();
    pal.setBrush(backgroundRole(), QBrush(QImage(BG_IMAGE_DEFAULT_1)));
    setPalette(pal);

    setAcceptDrops(true);
    setAutoFillBackground(true);
    setMinimumSize(DEFAULT_SIZE);
    setWindowTitle(tr("sempathored"));
}

void DragWidget::loadTextFile(const QString &sFilename, bool bColorsOn)
{
    QFile dictionaryFile(sFilename);
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
    QColor titleColor(Qt::white);
    int iColorCounter(0);
    QString line;
    do {
        line = inputStream.readLine();
        if (!line.isEmpty()) {
            if(bColorsOn) {
                //colorful titles in example
                switch(iColorCounter++ % 5) {
                 case 0: titleColor = Qt::white; break;
                 case 1: titleColor = Qt::lightGray; break;
                 case 2: titleColor = Qt::green; break;
                 case 3: titleColor = QColor(254,154,46); break;
                 case 4:
                 default: titleColor = Qt::red; break;
                }
            }
            DragLabel *wordLabel = new DragLabel(line, this, titleColor);
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
        DragLabel *wordLabel = new DragLabel("label", this);
        wordLabel->move(event->pos());
        wordLabel->show();
        wordLabel->setAttribute(Qt::WA_DeleteOnClose);
        wordLabel->editSlot();
    } else if (selectedAction == m_NewSquareAction) {
        DragSquare *wordSqare = new DragSquare("sqare","comments", this);
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

void DragWidget::loadUserBackgroundImage()
{
    QString supportedFormats("");
    foreach(QByteArray name,QImageWriter::supportedImageFormats())
        supportedFormats +=  name + " ";
    QString sFilename = QFileDialog::getOpenFileName(this, "Load background image: " + supportedFormats,"",
                                                     tr("Images (*.png *.xpm *.jpg)"));
    if(sFilename.size())
        changeBackgroundImage(sFilename);
}

void DragWidget::changeBackgroundColor(const QColor &acolor)
{
    QPalette pal = palette();
    pal.setBrush(backgroundRole(), acolor);
    setPalette(pal);
    //setStyleSheet(QString("background-image: " + sFilename + ";"));
    //setStyleSheet("background-color: rgb(85, 170, 255)");
}

void DragWidget::changeBackgroundImage(const QString  &sFilename)
{
    QString supportedFormats("");
    foreach(QByteArray name,QImageWriter::supportedImageFormats())
        supportedFormats +=  name + " ";
    if(sFilename.size()) {
        QByteArray ext = QFileInfo(sFilename).suffix().toLower().toLatin1();
        if(QImageWriter::supportedImageFormats().contains(ext)) {
            QImage newImage(sFilename);
            if(!newImage.isNull()) {
                QPalette pal = palette();
                pal.setBrush(backgroundRole(), QBrush(newImage));
                setPalette(pal);
            }  else {
                QMessageBox::warning(this,"Image does not exist or is not supported", "Your system supports only following formats: " +supportedFormats);
                return;
            }
        } else {
            QMessageBox::warning(this,"Image format is not supported", "Your system supports only following formats: " +supportedFormats);
        }
    }
}
