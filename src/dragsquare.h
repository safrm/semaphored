#ifndef DRAGSQUARE_H
#define DRAGSQUARE_H

#include "draglabel.h"

class DragSquare : public DragLabel
{
    Q_OBJECT
public:
    explicit DragSquare(const QString &text, QWidget *parent = 0);

signals:
    
public slots:
    
};

#endif // DRAGSQUARE_H
