#include <QPainter>
#include <QDebug>

#include "dragbaseline.h"
#include "dragwidget.h"

DragBaseLine::DragBaseLine(const QPoint & p1, const QPoint & p2, DragWidget *canvasWidget) :
    QWidget(canvasWidget),
    AbstractDragInterface(canvasWidget),
    m_AbsoluteLine(p1,p2),
    m_iLineWidth(LINE_WIDTH_2),
    m_iLineWidthToPaint(m_iLineWidth),
    m_LineStyle(Qt::SolidLine)
{
    qDebug() << "DragBaseLine() m_AbsoluteLine(" << p1 << p2 << ")";

    m_PaintingArea.setTop( qMin(p1.y(),p2.y()));
    m_PaintingArea.setBottom( qMax(p1.y(),p2.y()));
    m_PaintingArea.setLeft(qMin(p1.x(),p2.x()));
    m_PaintingArea.setRight(qMax(p1.x(),p2.x()));
    qDebug() << "DragBaseLine():m_PaintingArea topleft:" << m_PaintingArea.topLeft() << " bottomright:"<< m_PaintingArea.bottomRight();

    //relative to widget
    QPoint rel_p1(0,0), rel_p2(0,0);
    if(p1.x() <= p2.x())
        rel_p2.setX(p2.x() - p1.x());
    else
        rel_p1.setX(p1.x() - p2.x());

    if (p1.y() <= p2.y())
        rel_p2.setY( p2.y() - p1.y());
    else
        rel_p1.setY(p1.y() - p2.y());

    m_RelativeWidgetLine.setPoints(rel_p1, rel_p2);
    qDebug() << "DragBaseLine():m_RelativeWidgetLine " << m_RelativeWidgetLine.p1() << m_RelativeWidgetLine.p2();

    setAttribute( Qt::WA_TranslucentBackground, true );
    move(m_PaintingArea.left(), m_PaintingArea.top());
    resize(m_PaintingArea.size().width(), m_PaintingArea.size().height());
    qDebug() << "DragBaseLine():moving left:" << m_PaintingArea.left() << " top:"<< m_PaintingArea.top();
}

void DragBaseLine::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);
  QPen pen(Qt::black, m_iLineWidthToPaint, m_LineStyle);
  QPainter painter(this);
  painter.setPen(pen);
  painter.setBackgroundMode(Qt::TransparentMode);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);
  painter.drawLine(m_RelativeWidgetLine.p1(), m_RelativeWidgetLine.p2());
}

void DragBaseLine::select(bool bSelected)
{
  if (bSelected)
    m_iLineWidthToPaint = m_iLineWidth + LINE_WIDTH_SELECTED_HIGHLIGHT;
  else
    m_iLineWidthToPaint = m_iLineWidth;
  update();
}

QColor DragBaseLine::currentColor()
{
    return palette().color(backgroundRole());
}

void DragBaseLine::changeColor(const QColor &acolor)
{
    Q_UNUSED(acolor);
    //we don't want to change line colors for now
}
