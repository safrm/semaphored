/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://semaphored.safrm.net/
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

#ifndef YELLOWEDITBOX_H
#define YELLOWEDITBOX_H
#include <QLineEdit>

class DragLabel;
class QKeyEvent;
class YellowEditBox : public QLineEdit
{
    Q_OBJECT
public:
    explicit YellowEditBox(DragLabel *aParentLabel);
    void keyPressEvent(QKeyEvent *e);

signals:
    
public slots:
private:

   DragLabel *parentLabel;

public slots:
    void updateText();
    void cancel();

};

#endif // YELLOWEDITBOX_H
