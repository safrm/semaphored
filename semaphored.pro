HEADERS     = src/draglabel.h \
              src/dragwidget.h \
              src/dragsquare.h
RESOURCES   = src/semaphored.qrc
SOURCES     = src/draglabel.cpp \
              src/dragwidget.cpp \
              src/main.cpp \
              src/dragsquare.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/semaphored/semaphored
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.txt *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/semaphored/semaphored
INSTALLS += target sources

OTHER_FILES += \
    debian/changelog \
    debian/rules \
    debian/control \
    README \
    LICENSE.LGPL
