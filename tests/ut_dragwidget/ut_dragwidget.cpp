#include "ut_dragwidget.h"

void Ut_DragWidget::init()
{
    m_subject = new  DragWidget();
    m_subject->show();
}

void Ut_DragWidget::cleanup()
{
    delete m_subject;
    m_subject = NULL;
}

//just preparing test deployment
void Ut_DragWidget::test1()
{
	bool bTrue=1;
    QVERIFY(bTrue==1);
}
void Ut_DragWidget::testLoadSave_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("objectCount");
    QTest::newRow("screenshot 1")   << QString("../../screenshots/ss1.sem") << 10;
    QTest::newRow("screenshot 2")   << QString("../../screenshots/ss2.sem") << 10;
    QTest::newRow("screenshot 3")   << QString("../../screenshots/ss3.sem") << 10;
    QTest::newRow("screenshot 4")   << QString("../../screenshots/ss4.sem") << 10;
}

void Ut_DragWidget::testLoadSave()
{
    QFETCH(QString, filename);
    QFETCH(int, objectCount);
    m_subject->loadProject(filename);
    QCOMPARE(objectCount, m_subject->dragableObjectsCount());
    m_subject->saveProject(filename + ".testLoadSave");
    QCOMPARE(objectCount, m_subject->dragableObjectsCount());

    QProcess p;
    p.start("/usr/bin/diff", QStringList()
            << filename << filename + ".testLoadSave" );
    if ( !p.waitForStarted() )
        QFAIL( "process start failed" );

    if ( !p.waitForFinished() )
        QFAIL( "process finish failed" );

    QVERIFY( p.exitCode() == 0 );
}

QTEST_MAIN(Ut_DragWidget)

