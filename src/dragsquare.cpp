#include "dragsquare.h"

DragSquare::DragSquare(const QString &text, QWidget *parent)
   : QLabel(text, parent)
{
    QPalette pal = palette();
    pal.setColor(backgroundRole(), Qt::red);
    setPalette(pal);

    setAutoFillBackground(true);

    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Raised);
}
