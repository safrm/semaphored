#include "dragline.h"
#include <QPainter>
#include <QtGlobal>

DragLine::DragLine(const QPoint & p1, const QPoint & p2, QWidget *parent) :
    QWidget(parent)
{

    m_Area.setTop( qMin(p1.y(),p2.y()));
    m_Area.setBottom( qMax(p1.y(),p2.y()));
    m_Area.setLeft(qMin(p1.x(),p2.x()));
    m_Area.setRight(qMax(p1.x(),p2.x()));
    //relative to widget
    if(p1.x() <= p2.x()) {
        m_LineStart.setX(0);
        m_LineEnd.setX(p2.x() - p1.x());
    }
    else {
        m_LineStart.setX(p1.x() -p2.x());
        m_LineEnd.setX(0);
    }

    if(p1.y() <= p2.y()) {
        m_LineStart.setY(0);
        m_LineEnd.setY( p2.y()- p1.y());

    }
    else {
        m_LineStart.setY(p1.y() - p2.y());
        m_LineEnd.setY(0);
    }

    setAttribute( Qt::WA_TranslucentBackground, true );
    //resize(parentWidget()->size());
    move(m_Area.left(), m_Area.top());
    resize(m_Area.size().width(), m_Area.size().height());
 /*   setWindowOpacity(0.0);
    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::transparent); //Qt::blue
    setPalette(p);
    */

}

void DragLine::paintEvent(QPaintEvent *event)
{
  QPen pen(Qt::black, 2, Qt::SolidLine);
  QPainter painter(this);
  painter.setPen(pen);
  painter.setBackgroundMode(Qt::TransparentMode);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);
  painter.drawLine(m_LineStart, m_LineEnd);
}

