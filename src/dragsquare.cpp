#include "dragsquare.h"

DragSquare::DragSquare(const QString &text, QWidget *parent)
   : DragLabel(text, parent)
{
    setFrameShape(QFrame::Box);
    changeColor(Qt::red);
}
