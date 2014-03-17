#ifndef DRAGBASICLINE_H
#define DRAGBASICLINE_H

#include <QWidget>
#include <QLine>
#include "abstractdraginterface.h"

class DragBaseLine : public QWidget, public AbstractDragInterface
{
    Q_OBJECT
    Q_INTERFACES(AbstractDragInterface)

public:
    explicit DragBaseLine(const QPoint & p1, const QPoint & p2, DragWidget *parent);

    virtual QColor currentColor();
    virtual void changeColor(const QColor &acolor);
    virtual void select(bool bSelected);
    QLine absoluteLine() { return m_AbsoluteLine; }
    QLine relativeLine() { return m_RelativeWidgetLine; }

    enum { LINE_WIDTH_SELECTED_HIGHLIGHT = 2 };
    enum { LINE_WIDTH_2 = 2,
           LINE_WIDTH_4 = 4,
           LINE_WIDTH_8 = 8
         };
protected:
    void paintEvent(QPaintEvent *event);

    QLine m_AbsoluteLine;
    QLine m_RelativeWidgetLine;//relative to painting/selection area
    QRect m_PaintingArea;
    int m_iLineWidth, m_iLineWidthToPaint;
    Qt::PenStyle m_LineStyle;
};

#endif // DRAGBASICLINE_H
