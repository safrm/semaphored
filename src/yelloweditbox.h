#ifndef YELLOWEDITBOX_H
#define YELLOWEDITBOX_H
#include <QLineEdit>

class DragLabel;
class QKeyEvent;
class YellowEditBox : public QLineEdit
{
    Q_OBJECT
public:
    explicit YellowEditBox(DragLabel *aParentLabel);
    void keyPressEvent(QKeyEvent *e);

signals:
    
public slots:
private:

   DragLabel *parentLabel;

public slots:
    void updateText();
    void cancel();

};

#endif // YELLOWEDITBOX_H
