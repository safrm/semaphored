#ifndef ABSTRACTDRAGINTERFACE_H
#define ABSTRACTDRAGINTERFACE_H
#include <QColor>
#include <QString>

QT_BEGIN_NAMESPACE
class QMenu;
class  DragWidget;
class QDomElement;
QT_END_NAMESPACE

class AbstractDragInterface
{
public:
    explicit AbstractDragInterface(DragWidget* canvasWidget);
    virtual ~AbstractDragInterface() {}

    virtual QColor currentColor() = 0;
    virtual void changeColor(const QColor &acolor) = 0;
    virtual void select(bool bSelected) = 0;
    virtual void fillXmlElement(QDomElement & element) = 0;

    void setTimeStamp(qint64 mSecsSinceEpoch);
    const QString creationTimeStampString();
    const qint64 creationTimeStamp();

    enum { BORDER_WIDTH_NO_SELECTED = 2,
           BORDER_WIDTH_SELECTED  = 4};

    protected:
        DragWidget* m_CanvasWidget;
        qint64 m_i64TimeStamp;
        static QString TIMESTAMP_FORMAT;
};
Q_DECLARE_INTERFACE(AbstractDragInterface, "semaphored.AbstractDragInterface/1.0")

#endif // ABSTRACTDRAGINTERFACE_H
