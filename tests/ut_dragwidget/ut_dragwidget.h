
#ifndef UT_DRAGWIDGET_H
#define UT_DRAGWIDGET_H

#include <QtTest/QtTest>
#include <QObject>

#include <dragwidget.h>

class Ut_DragWidget : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void test1();
    void testLoadSave_data();
    void testLoadSave();
private:
    DragWidget *m_subject;
};

#endif // UT_DRAGWIDGET_H

