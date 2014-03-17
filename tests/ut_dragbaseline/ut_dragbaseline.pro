include(../common_tests.pri)
TARGET = ut_dragbaseline

INCLUDEPATH+= .

SOURCES += \
    ut_dragbaseline.cpp \
    dragwidget_stub.cpp \
    shapes/abstractdraginterface.cpp \
    shapes/dragbaseline.cpp

HEADERS += \
    ut_dragbaseline.h \
    dragwidget.h \
    shapes/abstractdraginterface.h \
    shapes/dragbaseline.h

