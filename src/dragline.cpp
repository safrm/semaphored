#include "dragline.h"
#include <QPainter>

DragLine::DragLine(const QPoint & p1, const QPoint & p2, QWidget *parent) :
    QWidget(parent)
{
    m_LineStart = p1;
    m_LineEnd = p2;
    setAttribute( Qt::WA_TranslucentBackground, true );
    resize(parentWidget()->size());
 /*   setWindowOpacity(0.0);
    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::transparent); //Qt::blue
    setPalette(p);
    */

}

void DragLine::paintEvent(QPaintEvent *event)
{
  //resize(m_LineEnd.x() - m_LineStart.x(),m_LineEnd.y()- m_LineStart.y());
  QPen pen(Qt::black, 2, Qt::SolidLine);
  QPainter painter(this);
  painter.setPen(pen);
  painter.setBackgroundMode(Qt::TransparentMode);
  painter.setRenderHint(QPainter::HighQualityAntialiasing	);
  painter.drawLine(m_LineStart, m_LineEnd);
}

