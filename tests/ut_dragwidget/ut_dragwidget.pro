include(../common_tests.pri)
include($$SRCDIR/shapes/shapes.pri)
TARGET = ut_dragwidget
INCLUDEPATH+= .

SOURCES += \
        ut_dragwidget.cpp \
        $$SRCDIR/dragwidget.cpp \
        $$SRCDIR/yelloweditbox.cpp \
        $$SRCDIR/version.cpp

HEADERS += \
        ut_dragwidget.h \
        $$SRCDIR/dragwidget.h \
        $$SRCDIR/dragwidget.h \
        $$SRCDIR/version.h \
        $$SRCDIR/yelloweditbox.h
