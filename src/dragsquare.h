#ifndef DRAGSQUARE_H
#define DRAGSQUARE_H

#include <QFrame>
class DragLabel;
class QTextEdit;
class QColor;
class DragSquare : public QFrame
{
    Q_OBJECT
public:
    explicit DragSquare(const QString &label, const QString &text, QWidget *parent = 0);
    QString label();
    QString text();
    QColor currentColor();
    void changeColor(const QColor &acolor);


public slots:
    void updateColorsSlot();
    void editLabelSlot() ;
protected :
    virtual void contextMenuEvent ( QContextMenuEvent * event );
private:

        DragLabel* labelDragLabel;
        QTextEdit* textTextEdit;
    
};

#endif // DRAGSQUARE_H
