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

#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>
QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QMenu;
class QAction;
class QRubberBand;
class DragLabel;

QT_END_NAMESPACE

class DragWidget : public QWidget
{
    Q_OBJECT
public:
    DragWidget(QWidget *parent = 0);
    enum { WHITE =  0,
           RED   =  1,
           ORANGE = 2,
           GREEN  = 3} color;
    static QString BG_IMAGE_DEFAULT_1;
    static QString BG_IMAGE_DEFAULT_2;
    static QString BG_IMAGE_KANBAN_1;
    static QString BG_IMAGE_KANBAN_1H;
    static QString BG_IMAGE_KANBAN_2;
    static QString BG_IMAGE_KANBAN_2H;
    static QString BG_IMAGE_KANBAN_3;
    static QString BG_IMAGE_KANBAN_3H;
    static QString BG_IMAGE_REVIEW_1;
    static QString BG_IMAGE_REVIEW_2;
    static QSize DEFAULT_SIZE;
    static QSize MINIMUM_SIZE;
    static int SIZE_A4_SHORT;
    static int SIZE_A4_LONG;
    static int SIZE_A5_SHORT;
    static int SIZE_A5_LONG;
    void changeBackgroundColor(const QColor &acolor);
    void changeBackgroundImage(const QString  &sFilename);
    void loadUserBackgroundImage();
    void loadTextFile(const QString &sFilename, bool bColorsOn = false);
    void loadProject(const QString &sFilename);
    void saveProject(const QString &sFilename);
    bool isMultiselecting();
    void deleteMutliselected();
    void changeColorMutliselected(const QColor &acolor);
    void exportToPicture(const QString &sFilename);

public slots:
    void deleteAllItemsSlot();
    void setFixedSizeBg(bool bFixed);

signals:
    void changeFixedSize(bool);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void contextMenuEvent ( QContextMenuEvent * event );
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent * event );

private:

    QMenu* rightClickMenu();
    static QMenu* m_RightClickMenu;

    QAction* m_NewLabelAction;
    QAction* m_NewSquareAction;
    QAction* m_NewLineAction;

    QList<QWidget*> selectedItems;
    QPoint m_selectionStartPoint;
    QPoint m_selectionEndPoint;
    QRubberBand* multiselectRubberBand;
    bool m_bPaintLine;
    QString m_BackgroundPicture; // "" - default, filename = picture, QColor::isValidColor  - color
    bool m_bFixedBgSize;
};

#endif
