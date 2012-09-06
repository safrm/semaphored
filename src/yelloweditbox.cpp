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

#include <QKeyEvent>
#include "draglabel.h"
#include "yelloweditbox.h"

YellowEditBox::YellowEditBox(DragLabel *aParentLabel) :
    QLineEdit( aParentLabel->parentWidget()),
    parentLabel(aParentLabel)
{
    setAttribute(Qt::WA_DeleteOnClose);
    QPalette p = palette();
    p.setColor(QPalette::Base,QColor("yellow"));
    setPalette(p);
    setText(parentLabel->text());
    setMinimumWidth(parentLabel->width()*2);
    move(parentLabel->pos());
    selectAll();
    connect(this,SIGNAL(editingFinished()),this,SLOT(updateText()));
}

void YellowEditBox::updateText()
{
    parentLabel->setText(text());
    parentLabel->adjustSize();
    close();
}

void YellowEditBox::cancel()
{
}

void YellowEditBox::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Escape)
      close();
  else
      QLineEdit::keyPressEvent(e);
}

