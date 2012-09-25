#ifndef ABSTRACTDRAGINTERFACE_H
#define ABSTRACTDRAGINTERFACE_H
#include <QColor>
class  DragWidget;

class AbstractDragInterface
{
public:
    explicit AbstractDragInterface(DragWidget* canvasWidget);
    virtual ~AbstractDragInterface() { };

    virtual QColor currentColor() = 0;
    virtual void changeColor(const QColor &acolor) = 0;
    virtual void select(bool bSelected) = 0;

    enum { BORDER_WIDTH_NO_SELECTED = 2,
           LINE_WIDTH_NO_SELECTED = 3,
           BORDER_WIDTH_SELECTED  = 4,
           LINE_WIDTH_SELECTED = 6 };
    protected:
        DragWidget* m_CanvasWidget;


};
Q_DECLARE_INTERFACE(AbstractDragInterface, "semaphored.AbstractDragInterface/1.0")

#endif // ABSTRACTDRAGINTERFACE_H
