HEADERS     = draglabel.h \
              dragwidget.h
RESOURCES   = semaphored.qrc
SOURCES     = draglabel.cpp \
              dragwidget.cpp \
              main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/semaphored/semaphored
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.txt *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/semaphored/semaphored
INSTALLS += target sources

symbian {
    TARGET.UID3 = 0xA000CF64
    include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
}
