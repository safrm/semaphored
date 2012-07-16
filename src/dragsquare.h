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

#ifndef DRAGSQUARE_H
#define DRAGSQUARE_H

#include <QFrame>
class DragLabel;
class QTextEdit;
class QColor;
class DragSquare : public QFrame
{
    Q_OBJECT
public:
    explicit DragSquare(const QString &label, const QString &text, QWidget *parent = 0, const QColor &defaultColor = Qt::white);
    QString label();
    QString text();
    QColor currentColor();
    void changeColor(const QColor &acolor);


public slots:
    void updateColorsSlot();
    void editLabelSlot() ;
protected :
    virtual void contextMenuEvent ( QContextMenuEvent * event );
private:

        DragLabel* labelDragLabel;
        QTextEdit* textTextEdit;
    
};

#endif // DRAGSQUARE_H
