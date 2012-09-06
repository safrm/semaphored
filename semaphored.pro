include(common.pri)
TARGET      = semaphored
HEADERS     = src/draglabel.h \
              src/dragwidget.h \
              src/dragsquare.h \
              src/mainwindow.h \
              src/yelloweditbox.h \
              src/aboutdialog.h \
              src/weblink.h \
              src/version.h
RESOURCES   = src/semaphored.qrc
SOURCES     = src/draglabel.cpp \
              src/dragwidget.cpp \
              src/main.cpp \
              src/dragsquare.cpp \
              src/mainwindow.cpp \
              src/yelloweditbox.cpp \
              src/aboutdialog.cpp \
              src/weblink.cpp \
              src/version.cpp

OBJECTS_DIR = build
MOC_DIR     = build
RCC_DIR     = build
UI_DIR      = build

# install
target.path = $$[QT_INSTALL_EXAMPLES]/$${TARGET}/$${TARGET}
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.txt *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/$${TARGET}/$${TARGET}
INSTALLS += target sources

OTHER_FILES += \
    debian/changelog \
    debian/rules \
    debian/control \
    README \
    LICENSE.LGPL \
    TODO \
    src/words.txt
