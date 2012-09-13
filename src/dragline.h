#ifndef DRAGLINE_H
#define DRAGLINE_H

#include <QWidget>
#include <QRect>

class DragLine : public QWidget
{
    Q_OBJECT

    public:
      DragLine(const QPoint & p1, const QPoint & p2, QWidget *parent = 0);

    protected:
      void paintEvent(QPaintEvent *event);
    private:
      QPoint m_LineStart;
      QPoint m_LineEnd;
      QRect m_Area;
    
};

#endif // DRAGLINE_H
