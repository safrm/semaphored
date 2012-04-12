#ifndef DRAGSQUARE_H
#define DRAGSQUARE_H

#include <QLabel>

class DragSquare : public QLabel
{
    Q_OBJECT
public:
    explicit DragSquare(const QString &text, QWidget *parent = 0);

signals:
    
public slots:
    
};

#endif // DRAGSQUARE_H
