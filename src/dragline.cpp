#include "dragline.h"
#include <QPainter>
#include <QtGlobal>

DragLine::DragLine(const QPoint & p1, const QPoint & p2, QWidget *parent) :
    QWidget(parent),
    m_p1(p1),
    m_p2(p2),
    m_iLineWidth(LINE_WIDTH_NO_SELECTED)
{

    m_PaintingArea.setTop( qMin(p1.y(),p2.y()));
    m_PaintingArea.setBottom( qMax(p1.y(),p2.y()));
    m_PaintingArea.setLeft(qMin(p1.x(),p2.x()));
    m_PaintingArea.setRight(qMax(p1.x(),p2.x()));
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
    move(m_PaintingArea.left(), m_PaintingArea.top());
    resize(m_PaintingArea.size().width(), m_PaintingArea.size().height());
}

void DragLine::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);
  QPen pen(Qt::black, m_iLineWidth, Qt::SolidLine);
  QPainter painter(this);
  painter.setPen(pen);
  painter.setBackgroundMode(Qt::TransparentMode);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);
  painter.drawLine(m_LineStart, m_LineEnd);
}
void DragLine::changeColor(const QColor &acolor)
{
    Q_UNUSED(acolor);
    //we don't want to change line colors for now
}

void DragLine::select(bool bSelected)
{
  if (bSelected)
    m_iLineWidth = LINE_WIDTH_SELECTED;
  else
    m_iLineWidth = LINE_WIDTH_NO_SELECTED;
  update();
}

QColor DragLine::currentColor()
{
    return palette().color(backgroundRole());
}
