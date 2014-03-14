
#ifndef UT_DRAGWIDGET_H
#define UT_DRAGWIDGET_H

#include <QtTest/QtTest>
#include <QObject>

//class EBTextEdit;
class Ut_DragWidget : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void test1();

};

#endif // UT_DRAGWIDGET_H

