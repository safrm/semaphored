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
    static QSize DEFAULT_SIZE;
    void changeBackgroundColor(const QColor &acolor);
    void changeBackgroundImage(const QString  &sFilename);
    void loadUserBackgroundImage();
    void loadTextFile(const QString &sFilename, bool bColorsOn = false);
    void loadProject(const QString &sFilename);
    void saveProject(const QString &sFilename);

public slots:
    void deleteAllItemsSlot();

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void contextMenuEvent ( QContextMenuEvent * event );

private:

    QMenu* rightClickMenu();
    static QMenu* m_RightClickMenu;

    QAction* m_NewLabelAction;
    QAction* m_NewSquareAction;

    QList<DragLabel*> selectedItems;
    QPoint selectionStart;
    QRubberBand* rubberBand;
};

#endif
