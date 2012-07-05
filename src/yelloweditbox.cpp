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

