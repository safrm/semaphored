
#include <QWidget>
#include <QLine>
#include "ut_dragbaseline.h"
#include "dragbaseline.h"
#include "dragwidget.h"

DragWidget *dragWidget = NULL;

void Ut_DragBaseLine::initTestCase()
{
    dragWidget = new  DragWidget();
    dragWidget->show();
}

void Ut_DragBaseLine::cleanupTestCase()
{
    delete dragWidget;
}

void Ut_DragBaseLine::init()
{
    //m_subject = new DragBaseLine(QPoint(100,100),QPoint(200,200), dragWidget);
    //QApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
}


void Ut_DragBaseLine::cleanup()
{
    //delete m_subject;
    //m_subject = 0;
}

void Ut_DragBaseLine::testConstructionAndDestruction()
{
    QPoint p1(100,100), p2(500,500);
    DragBaseLine* tmpLine = new DragBaseLine(p1,p2, dragWidget);
    QCOMPARE(tmpLine->absoluteLine().p1(),p1);
    QCOMPARE(tmpLine->absoluteLine().p2(),p2);
    tmpLine->show();

    dragWidget->repaint();
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    delete tmpLine;
}

void Ut_DragBaseLine::testConstruction_data()
{
    QTest::addColumn<QLine>("line");
    QTest::newRow("[0,0] - [200,200]")   << QLine(0,0,200,200);
    QTest::newRow("[200,200] - [0,0]")   << QLine(200,200,0,0);
    QTest::newRow("[0,200] - [200,0]")   << QLine(0,200,200,0);
    QTest::newRow("[200,0] - [0,200]")   << QLine(200,0,0,200);

    QTest::newRow("[200,200] - [400,400]")   << QLine(200,200,400,400);
    QTest::newRow("[400,400] - [200,200]")   << QLine(400,400,200,200);
    QTest::newRow("[200,400] - [400,200]")   << QLine(200,400,400,200);
    QTest::newRow("[400,200] - [200,400]")   << QLine(400,200,200,400);
}


void Ut_DragBaseLine::testConstruction()
{
    QFETCH(QLine, line);
    m_subject = new DragBaseLine(line.p1(),line.p2(), dragWidget);
    m_subject->show();
    dragWidget->repaint();
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    QCOMPARE(m_subject->absoluteLine().p1(),line.p1());
    QCOMPARE(m_subject->absoluteLine().p2(),line.p2());


    delete m_subject;
    m_subject = NULL;
}


QTEST_MAIN(Ut_DragBaseLine)

