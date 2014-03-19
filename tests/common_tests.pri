include(../common.pri)
SRCDIR = ../../src
STUBSDIR = ../stubs
TESTS_DIR = ../
INCLUDEPATH += $$SRCDIR $$STUBSDIR $$TESTS_DIR $$SRCDIR/shapes
DEPENDPATH = $$INCLUDEPATH
QT += testlib core gui xml
TEMPLATE = app
DEFINES += UNIT_TEST
MOC_DIR = ./.moc
CONFIG += debug testcase

# install tests
#target.path = $$[QT_INSTALL_LIBS]/tests
#INSTALLS += target

