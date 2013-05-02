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

#include <unistd.h>

#include "draglabel.h"
#include "dragwidget.h"
#include "dragsquare.h"
#include "dragline.h"
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

bool dtscomp(QDomElement& left, QDomElement & right) {
  return left.attribute("created").toLongLong() < right.attribute("created").toLongLong();
}


DragWidget::DragWidget(QWidget *parent)
    : QWidget(parent),
     m_NewLabelAction(NULL),
     m_NewSquareAction(NULL),
     m_NewLineAction(NULL),
     selectedItems(),
     m_selectionStartPoint(),
     m_selectionEndPoint(),
     multiselectRubberBand(NULL),
     m_bPaintLine(false),
     m_BackgroundPicture(""),
     m_bFixedBgSize(false)
{
    loadTextFile(QString(":/texts/words.txt"), true);

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
            usleep(1000); //to have unique TS
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

    if (event->mimeData()->hasText() || event->mimeData()->hasFormat("application/p1-hotspot")) {
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

    selectedItems.clear(); //they are anyway deleted

    if (event->mimeData()->hasText()) {
        const QMimeData *mime = event->mimeData();
        QPoint position = event->pos();
        QPoint hotSpot;
        QColor color(Qt::white);
        QByteArray timestamp = mime->data("application/timestamp");

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
            if(!timestamp.isEmpty())
                newSquare->setTimeStamp(timestamp.toLongLong());
            newSquare->move(position - hotSpot);
            newSquare->changeColor(color);
            newSquare->show();
            newSquare->setAttribute(Qt::WA_DeleteOnClose);
            position += QPoint(newSquare->width(), 0);
        } else { //label
          DragLabel *newLabel = new DragLabel(label, this, this);
          if(!timestamp.isEmpty())
              newLabel->setTimeStamp(timestamp.toLongLong());
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
    } else if (event->mimeData()->hasFormat("application/p1-hotspot")) {
       //for now we handle line separately
       const QMimeData *mime = event->mimeData();
       QByteArray timestamp = mime->data("application/timestamp");
       QPoint position = event->pos();
       QPoint hotSpot, p1, p2;

       QList<QByteArray> hotSpotPos = mime->data("application/x-hotspot").split(' ');
       if (hotSpotPos.size() == 2) {
           hotSpot.setX(hotSpotPos.first().toInt());
           hotSpot.setY(hotSpotPos.last().toInt());
       }
       QList<QByteArray> p1Pos = mime->data("application/p1-hotspot").split(' ');
       if (hotSpotPos.size() == 2) {
           p1.setX(p1Pos.first().toInt());
           p1.setY(p1Pos.last().toInt());
       }
       QList<QByteArray> p2Pos = mime->data("application/p2-hotspot").split(' ');
       if (hotSpotPos.size() == 2) {
           p2.setX(p2Pos.first().toInt());
           p2.setY(p2Pos.last().toInt());
       }
       QPoint movingDiff = position - hotSpot - p1;
       DragLine *newLine = new DragLine(p1+movingDiff, p2+movingDiff, this);
       if(!timestamp.isEmpty())
           newLine->setTimeStamp(timestamp.toLongLong());
       newLine->show();
       newLine->setAttribute(Qt::WA_DeleteOnClose);
       position += QPoint(newLine->width(), 0);
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
    if (m_bPaintLine) {
        m_selectionStartPoint = event->pos();
        m_selectionEndPoint = event->pos();
        return;
    }

    QWidget * widget = childAt(event->pos());
    //we pressed out of our objects
    if (!widget) { //|| !widget->inherits("YellowEditBox")) {
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
          foreach (QWidget *widget, selectedItems) {
                  if (widget->inherits("AbstractDragInterface")) {
                       AbstractDragInterface* abstractDrag = qobject_cast<AbstractDragInterface*>(widget);
                       abstractDrag->select(false);
              }
          }
           selectedItems.clear();
           return;
        } else {
            //we don't have selection so we try to create it
            m_selectionStartPoint = event->pos();
            if (!multiselectRubberBand) {
                  multiselectRubberBand = new QRubberBand(QRubberBand::Rectangle, this);
                  Q_ASSERT(multiselectRubberBand);
            }
            if (!multiselectRubberBand->isVisible()) {
                //multiselectRubberBand->setGeometry(QRect(m_selectionStartPoint, QSize(1,1)).normalized());
                multiselectRubberBand->setGeometry(m_selectionStartPoint.x(),m_selectionStartPoint.y(),0,0);
                multiselectRubberBand->show();
            }
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
    DragLine *lineChild(NULL);

    //1 label
    if (widget->inherits("DragLabel"))
        labelChild = static_cast<DragLabel*>(widget);
    else if (widget->inherits("DragSquare"))
        squareChild = static_cast<DragSquare*>(widget);
    else if (widget->inherits("DragLine"))
        lineChild = static_cast<DragLine*>(widget);


    if(labelChild) {
        if(widget->parent()->inherits("DragSquare")) {
            squareChild = static_cast<DragSquare*>(labelChild->parent()); //we take the whole square
        } else {
          QPoint hotSpot = event->pos() - labelChild->pos();

            QMimeData *mimeData = new QMimeData;
            mimeData->setData("application/timestamp", QByteArray::number(labelChild->creationTimeStamp()));
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
        mimeData->setData("application/timestamp", QByteArray::number(squareChild->creationTimeStamp()));
        mimeData->setData("application/x-hotspot", QByteArray::number(hotSpot.x()) + " " + QByteArray::number(hotSpot.y()));
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
    if(lineChild) {
       QPoint hotSpot = event->pos() - lineChild->pos();

       QMimeData *mimeData = new QMimeData;
       mimeData->setData("application/timestamp", QByteArray::number(lineChild->creationTimeStamp()));
       mimeData->setData("application/x-hotspot", QByteArray::number(hotSpot.x()) + " " + QByteArray::number(hotSpot.y()));
       mimeData->setData("application/p1-hotspot", QByteArray::number(lineChild->p1().x()) + " " + QByteArray::number(lineChild->p1().y()));
       mimeData->setData("application/p2-hotspot",QByteArray::number(lineChild->p2().x()) + " " + QByteArray::number(lineChild->p2().y()));

       QPixmap pixmap(lineChild->size());
       pixmap.fill( Qt::transparent );
       lineChild->render(&pixmap);

       QDrag *drag = new QDrag(this);
       drag->setMimeData(mimeData);
       drag->setPixmap(pixmap);
       drag->setHotSpot(hotSpot);

       Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

       if (dropAction == Qt::MoveAction)
            lineChild->close();
    }

}
void DragWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bPaintLine) {
           m_selectionEndPoint = event->pos();
           update();
    } else
        if(multiselectRubberBand && multiselectRubberBand->isVisible())
            multiselectRubberBand->setGeometry(QRect(m_selectionStartPoint, event->pos()).normalized());
}

void DragWidget::mouseReleaseEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    if( m_bPaintLine) {
            DragLine *line = new DragLine(m_selectionStartPoint, event->pos(),this);
            line->show();
            line->setAttribute(Qt::WA_DeleteOnClose);
            m_bPaintLine = false;
    } else if( multiselectRubberBand && multiselectRubberBand->isVisible()) {
        multiselectRubberBand->hide();
        foreach (QObject *child, children()) {
            if (child->inherits("AbstractDragInterface")) {
                QWidget* widget = qobject_cast<QWidget*>(child);
                if(multiselectRubberBand->geometry().contains(widget->geometry())) {
                   selectedItems += widget;
                   AbstractDragInterface* abstractDrag = qobject_cast<AbstractDragInterface*>(child);
                   abstractDrag->select(true);
                }
            }
        }
    }
}

void DragWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    event->accept();
    QAction* selectedAction = rightClickMenu()->exec(event->globalPos());
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
    } else if (selectedAction == m_NewLineAction) {
        //we will paint line now
        m_bPaintLine = true;
    } else {
        qCritical("invalid action processed");
    }

}

void DragWidget::paintEvent(QPaintEvent *event)
{
   if(m_bPaintLine) {
        QPen pen(Qt::darkBlue, 3, Qt::DashLine);
        QPainter painter(this);
        painter.setPen(pen);
        //painter.setBackgroundMode(Qt::TransparentMode);
        //painter.setRenderHint(QPainter::HighQualityAntialiasing);
        painter.drawLine(m_selectionStartPoint, m_selectionEndPoint);
    }
    QWidget::paintEvent(event);
}

QMenu* DragWidget::rightClickMenu()
{
  if (!m_RightClickMenu) {
    m_RightClickMenu = new QMenu;
    QMenu* newMenu = new QMenu("New", m_RightClickMenu);

    m_NewLabelAction = new QAction(QIcon(":/icons/new_label.svg"), tr("&Label"), this);
    newMenu->addAction(m_NewLabelAction);

    m_NewSquareAction = new QAction(QIcon(":/icons/new_square.svg"), tr("&Square"), this);
    newMenu->addAction(m_NewSquareAction);

    m_NewLineAction = new QAction(QIcon(":/icons/new_line.svg"), tr("&Line"), this);
    newMenu->addAction(m_NewLineAction);


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
        if (child->inherits("DragLine")) {
            DragLine *widget = static_cast<DragLine *>(child);
            widget->deleteLater();
        }
    }
}

void DragWidget::deleteMutliselected()
{
    foreach (QWidget *widget, selectedItems) {
        AbstractDragInterface* abstractDrag = qobject_cast<AbstractDragInterface*>(widget);
        abstractDrag->select(false);
        if(widget->parentWidget()->inherits("DragSquare"))
            widget->parentWidget()->close();
         else
           widget->close();
   }
   selectedItems.clear();
}
void DragWidget::changeColorMutliselected(const QColor &acolor)
{
    foreach (QWidget *widget, selectedItems) {
         qobject_cast<AbstractDragInterface*>(widget)->changeColor(acolor);
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
    if (sFilename.size()) {
        QByteArray ext = QFileInfo(sFilename).suffix().toLower().toLatin1();
        if(QImageWriter::supportedImageFormats().contains(ext)) {
            QImage newImage(sFilename);
            if(!newImage.isNull()) {
                QPalette pal = palette();
                //QPixmap pixmap;
                //pixmap.load("image.jpg");
                //widget->setPixmap(pixmap);
                if (m_bFixedBgSize)
                    pal.setBrush(backgroundRole(), QBrush(newImage));
                else
                    pal.setBrush(backgroundRole(), QBrush(newImage.scaled(size(),Qt::IgnoreAspectRatio)));
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
    if(Version::shortVersionToNum(sVersion) != Version::shortVersionToNum(QString(APP_SHORT_VERSION_TAG))) {
        qCritical()<< "Project saved by:" + sVersion + "trying to load by:" + QString(APP_SHORT_VERSION_TAG);
    }

    //TODO - should be a bit more robust
    QDomElement itemsElem = doc.elementsByTagName("items").at(0).toElement();
    //background
    QString sBackground = itemsElem.attribute("background");
#if (QT_VERSION < QT_VERSION_CHECK(4, 7, 0))
    QColor TestedColor(sBackground);
    if (TestedColor.isValid())
#else
    if(QColor::isValidColor(sBackground))
#endif
       changeBackgroundColor(QColor(sBackground));
    else if (QFileInfo(sBackground).exists())
        changeBackgroundImage(sBackground);
    else  {
        qCritical()<< "Not recognized background color:"+ sBackground;
        changeBackgroundColor(Qt::white);
    }
    //TODO window position and size
    QString sW = itemsElem.attribute("w");
    QString sH = itemsElem.attribute("h");
    setFixedSize(sW.toInt(),sH.toInt());

    QString sFixedSize = itemsElem.attribute("fixed_size");
    setFixedSizeBg(sFixedSize.toInt()==1);


    //items loop
    QDomNode n = itemsElem.firstChild();

    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if (!e.isNull()) {
            qDebug()<< qPrintable(e.tagName())  << " : " << qPrintable(e.text());

            if (e.tagName() == "label") {
                DragLabel *wordLabel = new DragLabel(e.attribute("label"), this, this, QColor(e.attribute("color")));
                wordLabel->setTimeStamp(e.attribute("created").toLongLong());
                wordLabel->move(e.attribute("x").toInt(), e.attribute("y").toInt());
                wordLabel->show();
                wordLabel->setAttribute(Qt::WA_DeleteOnClose);
            } else if (e.tagName() == "square")  {
                DragSquare *wordSquare = new DragSquare(e.attribute("label"), e.attribute("text"), this, QColor(e.attribute("color")));
                wordSquare->setTimeStamp(e.attribute("created").toLongLong());
                wordSquare->move(e.attribute("x").toInt(), e.attribute("y").toInt());
                wordSquare->show();
                wordSquare->setAttribute(Qt::WA_DeleteOnClose);
            }  else if (e.tagName() == "line")  {
                DragLine *line = new DragLine(QPoint(e.attribute("p1x").toInt(),e.attribute("p1y").toInt()),QPoint(e.attribute("p2x").toInt(),e.attribute("p2y").toInt()), this);
                line->setTimeStamp(e.attribute("created").toLongLong());
                //line->move(e.attribute("x").toInt(), e.attribute("y").toInt());
                line->show();
                line->setAttribute(Qt::WA_DeleteOnClose);
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
    root.setAttribute("version", APP_SHORT_VERSION_TAG);
    xmlDocument.appendChild( root );

    QDomElement items = xmlDocument.createElement("items");
    items.setAttribute("background", m_BackgroundPicture );
    items.setAttribute("w", QString::number(size().width()));
    items.setAttribute("h", QString::number(size().height()));
    items.setAttribute("fixed_size", QString::number(m_bFixedBgSize));
    root.appendChild(items);

    QList<QDomElement> sortedList;
    QDomElement tag;
    foreach (QObject *child, children()) {
            if (child->inherits("DragLabel")) {
                DragLabel *widgetLabel = static_cast<DragLabel *>(child);
                tag = xmlDocument.createElement("label");
                tag.setAttribute("created", QString::number(widgetLabel->creationTimeStamp()));
                tag.setAttribute("color", widgetLabel->currentColor().name());
                tag.setAttribute("label", widgetLabel->text());
                tag.setAttribute("x", QString::number(widgetLabel->pos().x()));
                tag.setAttribute("y", QString::number(widgetLabel->pos().y()));
                sortedList.append(tag);
            }
            else if (child->inherits("DragSquare")) {
                DragSquare *widgetSquare = static_cast<DragSquare *>(child);
                tag = xmlDocument.createElement("square");
                tag.setAttribute("created", QString::number(widgetSquare->creationTimeStamp()));
                tag.setAttribute("color", widgetSquare->currentColor().name());
                tag.setAttribute("label", widgetSquare->text());
                tag.setAttribute("text", widgetSquare->text());
                tag.setAttribute("x", QString::number(widgetSquare->pos().x()));
                tag.setAttribute("y", QString::number(widgetSquare->pos().y()));
                sortedList.append(tag);
            }
            else if (child->inherits("DragLine")) {
                DragLine *widgetLine = static_cast<DragLine *>(child);
                tag = xmlDocument.createElement("line");
                tag.setAttribute("created", QString::number(widgetLine->creationTimeStamp()));
                tag.setAttribute("p1x", QString::number(widgetLine->p1().x()));
                tag.setAttribute("p1y", QString::number(widgetLine->p1().y()));
                tag.setAttribute("p2x", QString::number(widgetLine->p2().x()));
                tag.setAttribute("p2y", QString::number(widgetLine->p2().y()));
                sortedList.append(tag);
            }
            else {
                qCritical( "Unknown AbstractDragInterface type");
            }
    }
    //to keep as small diff in git as possible
    qSort(sortedList.begin(), sortedList.end(), dtscomp);
    foreach(tag, sortedList)
      items.appendChild(tag);
    //save
    out << xmlDocument.toString();
}

bool DragWidget::isMultiselecting()
{
    return selectedItems.size() != 0 ;
}

void DragWidget::resizeEvent ( QResizeEvent * event )
{
    QPalette pal = palette();
    if (m_bFixedBgSize)
        pal.setBrush(backgroundRole(), QBrush(QImage(m_BackgroundPicture)));
    else
        pal.setBrush(backgroundRole(), QBrush(QImage(m_BackgroundPicture).scaled(size(),Qt::IgnoreAspectRatio)));
    setPalette(pal);
    QWidget::resizeEvent(event);
}

void DragWidget::setFixedSizeBg(bool bFixed)
{
    m_bFixedBgSize = bFixed;
    if (m_bFixedBgSize)
        setFixedSize(size());
    else {
        setMinimumSize(MINIMUM_SIZE);
        setMaximumSize(QSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX));
    }
    emit changeFixedSize(bFixed);
}
