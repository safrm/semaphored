
#ifndef UT_DRAGBASELINE_H
#define UT_DRAGBASELINE_H

#include <QtTest/QtTest>
#include <QObject>

#include <dragbaseline.h>

class Ut_DragBaseLine : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testConstructionAndDestruction();
    void testConstruction_data();
    void testConstruction();
private:
    DragBaseLine *m_subject;
};

#endif // UT_DRAGBASELINE_H

