#ifndef DRAGLINE_H
#define DRAGLINE_H

#include <QWidget>

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
    
};

#endif // DRAGLINE_H
