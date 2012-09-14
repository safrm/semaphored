#ifndef DRAGLINE_H
#define DRAGLINE_H

#include <QWidget>
#include <QRect>

class DragLine : public QWidget
{
    Q_OBJECT

    public:
      DragLine(const QPoint & p1, const QPoint & p2, QWidget *parent = 0);
      const QPoint p1() { return m_p1; }
      const QPoint p2() { return m_p2; }

    protected:
      void paintEvent(QPaintEvent *event);
    private:
      QPoint m_LineStart, m_LineEnd; //relative to painting/selection area
      QRect m_PaintingArea;
      QPoint m_p1, m_p2; //base dragwidget relative
    
};

#endif // DRAGLINE_H
