#ifndef DRAGLINE_H
#define DRAGLINE_H

#include <QWidget>
#include <QRect>
#include "abstractdraginterface.h"

class DragLine : public QWidget, public AbstractDragInterface
{
    Q_OBJECT
    Q_INTERFACES(AbstractDragInterface)

    public:
      DragLine(const QPoint & p1, const QPoint & p2, QWidget *parent = 0);
      const QPoint p1() { return m_p1; }
      const QPoint p2() { return m_p2; }

      //abstract behaviour
      virtual QColor currentColor();
      virtual void changeColor(const QColor &acolor);
      virtual void select(bool bSelected);

    protected:
      void paintEvent(QPaintEvent *event);
    private:
      QPoint m_LineStart, m_LineEnd; //relative to painting/selection area
      QRect m_PaintingArea;
      QPoint m_p1, m_p2; //base dragwidget relative
      int m_iLineWidth;
    
};

#endif // DRAGLINE_H
