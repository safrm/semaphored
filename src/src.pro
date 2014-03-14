include(../common.pri)
TARGET      = semaphored

SEMAPHORED_SRC_DIR=.
INCLUDEPATH+= . ./src
include(shapes/shapes.pri)

HEADERS     += \
    $$SEMAPHORED_SRC_DIR/mainwindow.h \
    $$SEMAPHORED_SRC_DIR/yelloweditbox.h \
    $$SEMAPHORED_SRC_DIR/aboutdialog.h \
    $$SEMAPHORED_SRC_DIR/weblink.h \
    $$SEMAPHORED_SRC_DIR/dragwidget.h \
    $$SEMAPHORED_SRC_DIR/version.h \
    $$SEMAPHORED_SRC_DIR/commandlineargs.h \
    $$SEMAPHORED_SRC_DIR/desktopfile.h

RESOURCES   = $$SEMAPHORED_SRC_DIR/semaphored.qrc
SOURCES     += \
    $$SEMAPHORED_SRC_DIR/main.cpp \
    $$SEMAPHORED_SRC_DIR/mainwindow.cpp \
    $$SEMAPHORED_SRC_DIR/yelloweditbox.cpp \
    $$SEMAPHORED_SRC_DIR/aboutdialog.cpp \
    $$SEMAPHORED_SRC_DIR/weblink.cpp \
    $$SEMAPHORED_SRC_DIR/dragwidget.cpp \
    $$SEMAPHORED_SRC_DIR/version.cpp \
    $$SEMAPHORED_SRC_DIR/abstractdraginterface.cpp \
    $$SEMAPHORED_SRC_DIR/commandlineargs.cpp \
    $$SEMAPHORED_SRC_DIR/desktopfile.cpp


OTHER_FILES += \
    $$SEMAPHORED_SRC_DIR/texts/words.txt \
    $$SEMAPHORED_SRC_DIR/demo-projects/words.txt


INSTALLS    += target
unix:target.path  = /usr/bin
win32:target.path  = /

compiler_clean.commands = -$(DEL_FILE) $(TARGET)
