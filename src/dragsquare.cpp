#include <QVBoxLayout>
#include <QTextEdit>
#include <QMenu>
#include <QContextMenuEvent>

#include "dragsquare.h"
#include "draglabel.h"

DragSquare::DragSquare(const QString &label, const QString &text, QWidget *parent)
    : QFrame(parent)
{

    setAutoFillBackground(true);
    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Raised);

    QVBoxLayout * mainLayout= new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    labelDragLabel = new DragLabel(label, this);
    connect( labelDragLabel, SIGNAL(colorChangedSignal()), this, SLOT(updateColorsSlot()));
    textTextEdit = new QTextEdit(text, this);
    //int iHeight = textTextEdit->fontPointSize();
    int iWidth = labelDragLabel->width();
    textTextEdit->setAutoFillBackground(true);
    textTextEdit->setFrameShape(QFrame::Box);
    textTextEdit->setMaximumSize(labelDragLabel->width(),60);
    mainLayout->addWidget(labelDragLabel);
    mainLayout->addWidget(textTextEdit);
    setLayout(mainLayout);
    updateColorsSlot();
}

void DragSquare::contextMenuEvent( QContextMenuEvent * event )
{
    event->accept();
    labelDragLabel->rightClickMenu()->exec(event->globalPos());
    //const QAction* selectedAction = rightClickMenu()->exec(event->globalPos());
}

//stupid way!!
void DragSquare::changeColor(const QColor &acolor)
{
    labelDragLabel->changeColor(acolor);
}

void DragSquare::updateColorsSlot()
{
    QPalette pal = palette();
    pal.setColor(backgroundRole(), labelDragLabel->currentColor());
    pal.setColor(QPalette::Base,labelDragLabel->currentColor());
    setPalette(pal);

    textTextEdit->setPalette(pal);
}

void DragSquare::editLabelSlot()
{
    labelDragLabel->editSlot();
}
QString DragSquare::label()
{
    return labelDragLabel->text();
}

QString DragSquare::text()
{
    return textTextEdit->toPlainText();
}

QColor DragSquare::currentColor()
{
    return labelDragLabel->currentColor();
}

