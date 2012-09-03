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
#include <QRubberBand>
#include <QDomDocument>
#include <QTextStream>
#include <QFile>

#include "draglabel.h"
#include "dragwidget.h"
#include "dragsquare.h"
#include "yelloweditbox.h"
#include "version.h"

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
     rubberBand(NULL),
     m_BackgroundPicture("")
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
    m_BackgroundPicture = BG_IMAGE_DEFAULT_1;

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
            DragLabel *wordLabel = new DragLabel(line, this, this, titleColor);
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
          DragLabel *newLabel = new DragLabel(label, this, this);
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
    if (event->button() == Qt::RightButton)
        return;
    QWidget * widget = childAt(event->pos());
    //we pressed out of our objects
    if (!widget) {
        //cancel YellowBox edit
        //a bit stupid way but easy.. maybe it could restore old text instead of applying new one?
        foreach (QObject *yellowBox, children()) {
            if (yellowBox->inherits("YellowEditBox")) {
                emit (static_cast<YellowEditBox*>(yellowBox))->updateText();
                return;
            }
        }
        //we have selection so we want to clean it
        if (isMultiselecting()) {
            foreach (DragLabel *widget, selectedItems)
               widget->select(false);
           selectedItems.clear();
           return;
        } else {
        //we don't have selection so we try to create it
        selectionStart = event->pos();
        if (!rubberBand)
              rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
          rubberBand->setGeometry(QRect(selectionStart, QSize(1,1)).normalized());
          rubberBand->show();
        return;
        }
    }

    //TODO we have selection and want to work with it
    if (isMultiselecting()) {
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
void DragWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(rubberBand)
        rubberBand->setGeometry(QRect(selectionStart, event->pos()).normalized());
}

void DragWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if(rubberBand && rubberBand->isVisible()) {
        rubberBand->hide();
        foreach (QObject *child, children()) {
            if (child->inherits("DragLabel")) {
                DragLabel *widget = static_cast<const DragLabel *>(child);
                if(rubberBand->geometry().contains(widget->geometry())) {
                    selectedItems += widget;
                    widget->select(true);
                }
            } else if (child->inherits("DragSquare")) {
                DragSquare *widgetSquare = static_cast<const DragSquare *>(child);
                DragLabel *widgetLabelFromSquare = widgetSquare->labelWidget();
                if(rubberBand->geometry().contains(widgetSquare->geometry())) {
                    selectedItems += widgetLabelFromSquare;
                    widgetLabelFromSquare->select(true);
                }
            }
        }
    }
}

void DragWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    event->accept();
    QPoint pos = event->globalPos();
    QAction* selectedAction = rightClickMenu()->exec(pos);
    if (selectedAction == m_NewLabelAction) {
        DragLabel *wordLabel = new DragLabel("label", this, this);
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
    selectedItems.clear();
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

void DragWidget::deleteMutliselected()
{
    foreach (DragLabel *widget, selectedItems) {
        widget->select(false);
        if(widget->parentWidget()->inherits("DragSquare"))
            widget->parentWidget()->close();
         else
           widget->close();
   }
   selectedItems.clear();
}
void DragWidget::changeColorMutliselected(const QColor &acolor)
{
    foreach (DragLabel *widget, selectedItems) {
         widget->changeColor(acolor);
    }
}

void DragWidget::loadUserBackgroundImage()
{
    QString supportedFormats("");
    foreach(QByteArray name,QImageWriter::supportedImageFormats())
        supportedFormats +=  name + " ";
    QString sFilename = QFileDialog::getOpenFileName(this, "Load background image: " + supportedFormats,"",
                                                     tr("Images (*.png *.xpm *.jpg)"));
    if(sFilename.size()) {
        changeBackgroundImage(sFilename);
        m_BackgroundPicture = sFilename;
    }
}

void DragWidget::changeBackgroundColor(const QColor &acolor)
{
    QPalette pal = palette();
    pal.setBrush(backgroundRole(), acolor);
    setPalette(pal);
    m_BackgroundPicture = acolor.name();
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
                m_BackgroundPicture = sFilename;
            }  else {
                QMessageBox::warning(this,"Image does not exist or is not supported", "Your system supports only following formats: " +supportedFormats);
                return;
            }
        } else {
            QMessageBox::warning(this,"Image format is not supported", "Your system supports only following formats: " +supportedFormats);
        }
    }
}

void DragWidget::loadProject(const QString &sFilename)
{
    //clean & delete
    deleteAllItemsSlot();
    QFile file(sFilename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"Loading project failed", "Check file : " + sFilename);
        return ;
    }
    QDomDocument doc( sFilename );
    QString errotMsg("");
    int iErrorLine(0), iErrorCol(0);
    if ( !doc.setContent( &file,false, &errotMsg, &iErrorLine, &iErrorCol))
    {
      file.close();
      qCritical()<< "QDomDocument::setContent: " <<  errotMsg << " Line " << iErrorLine << " column: " << iErrorCol;
      return;
    }
    file.close();

    //parsing
    QString tagName("");
    QDomElement docElem = doc.documentElement();

    //root element
    if( docElem.tagName() != "project") {
        qCritical()<< "Wrong root element " ;
        return;
    }
    //version check
    QString sVersion = docElem.attribute("version");
    if(Version::shortVersionToNum(sVersion) != Version::shortVersionToNum(QString(APP_VERSION_SHORT))) {
        qCritical()<< "Project saved by:" + sVersion + "trying to load by:" + QString(APP_VERSION_SHORT);
    }

    //TODO - should be a bit more robust
    QDomElement itemsElem = doc.elementsByTagName("items").at(0).toElement();
    //background
    QString sBackground = itemsElem.attribute("background");
    if(QColor::isValidColor(sBackground))
       changeBackgroundColor(QColor(sBackground));
    else if (QFileInfo(sBackground).exists())
        changeBackgroundImage(sBackground);
    else  {
        qCritical()<< "Not recognized background color:"+ sBackground;
        changeBackgroundColor(Qt::white);
    }
    //TODO window position and size
    //QString sX = itemsElem.attribute("x");
    //QString sY = itemsElem.attribute("y");
    QString sW = itemsElem.attribute("w");
    //setWidth(sW.toInt());
    QString sH = itemsElem.attribute("h");
    //setHeight(sH.toInt());

    //items loop
    QDomNode n = itemsElem.firstChild();

    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if (!e.isNull()) {
            qDebug()<< qPrintable(e.tagName())  << " : " << qPrintable(e.text());

            if (e.tagName() == "label") {
                DragLabel *wordLabel = new DragLabel(e.attribute("label"), this, this, QColor(e.attribute("color")));
                wordLabel->move(e.attribute("x").toInt(), e.attribute("y").toInt());
                wordLabel->show();
                wordLabel->setAttribute(Qt::WA_DeleteOnClose);
            } else if (e.tagName() == "square")  {
                DragSquare *wordSquare = new DragSquare(e.attribute("label"), e.attribute("text"), this, QColor(e.attribute("color")));
                wordSquare->move(e.attribute("x").toInt(), e.attribute("y").toInt());
                wordSquare->show();
                wordSquare->setAttribute(Qt::WA_DeleteOnClose);
            }
        }
        n = n.nextSibling();
    }
}

void DragWidget::saveProject(const QString &sFilename)
{
    QDomDocument xmlDocument; //has to be first because of EditorView::OnlyPlainData reads it
    QFile fileWrite(sFilename);
    if (!fileWrite.open(QFile::WriteOnly| QFile::Text)) {
        QMessageBox::warning(this,"Saving project failed", "Check file permissions: " + sFilename);
        return ;
    }
    QTextStream out(&fileWrite);
    QDomElement root = xmlDocument.createElement("project" );
    root.setAttribute("version", APP_VERSION_SHORT);
    xmlDocument.appendChild( root );

    QDomElement items = xmlDocument.createElement("items");
    items.setAttribute("background", m_BackgroundPicture );
    //items.setAttribute("geometry", QString(saveGeometry().toHex()));
    items.setAttribute("x", QString::number(pos().x()));
    items.setAttribute("y", QString::number(pos().y()));
    items.setAttribute("w", QString::number(size().width()));
    items.setAttribute("h", QString::number(size().height()));
    root.appendChild(items);

    QDomElement tag;
    foreach (QObject *child, children()) {
        if (child->inherits("DragLabel")) {
            DragLabel *widgetLabel = static_cast<DragLabel *>(child);
            tag = xmlDocument.createElement("label");
            tag.setAttribute("color", widgetLabel->currentColor().name());
            tag.setAttribute("label", widgetLabel->text());
            tag.setAttribute("x", QString::number(widgetLabel->pos().x()));
            tag.setAttribute("y", QString::number(widgetLabel->pos().y()));
            items.appendChild(tag);
        }
        else if (child->inherits("DragSquare")) {
            DragSquare *widgetSquare = static_cast<DragSquare *>(child);
            tag = xmlDocument.createElement("square");
            tag.setAttribute("color", widgetSquare->currentColor().name());
            tag.setAttribute("label", widgetSquare->text());
            tag.setAttribute("text", widgetSquare->text());
            tag.setAttribute("x", QString::number(widgetSquare->pos().x()));
            tag.setAttribute("y", QString::number(widgetSquare->pos().y()));
            items.appendChild(tag);
        }
        else
          ;
    }
   //save
    out << xmlDocument.toString();
}

bool DragWidget::isMultiselecting()
{
    return selectedItems.size() != 0;
}
