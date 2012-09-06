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
unix:target.path  = /usr/bin/
win32:target.path  = /
INSTALLS += target

unix:desktop.path  = /usr/share/applications
unix:desktop.files  = data/semaphored.desktop
unix:INSTALLS    += desktop

unix:icon.path   = /usr/share/icons/hicolor/scalable/apps
win32:icon.path  = /
unix:icon.files  = data/semaphored.svg
win32:icon.files  = data/semaphored.ico
INSTALLS    += icon

unix:readme.path  = /usr/share/doc/semaphored
win32:readme.path  = /
readme.files  = README
unix:readme.commands = sed -i".bkp" \'1 s/Version:.*/Version: $$APP_FULL_VERSION_TAG ($$APP_BUILD_DATE)/\' README && rm -f README.bkp
INSTALLS    += readme

unix:docs.path  = /usr/share/doc/semaphored
win32:docs.path  = /
docs.files  = TODO LICENSE.LGPL
INSTALLS    += docs

win32 {
QTDIR_BIN = $$system(qmake -query QT_INSTALL_BINS)
message($$QTDIR)
INSTALLS    += qtlibraries
qtlibraries.path  = /
qtlibraries.files = \
    $$QTDIR_BIN/mingwm10.dll \
    $$QTDIR_BIN/libgcc_s_dw2-1.dll

  release {
    message(libs:release)
    qtlibraries.files += \
      $$QTDIR_BIN/QtCore4.dll \
      $$QTDIR_BIN/QtGui4.dll \
      $$QTDIR_BIN/QtSvg4.dll \
      $$QTDIR_BIN/QtXml4.dll \
#      $$QTDIR_BIN/QtNetwork4.dll
  } 
  debug { 
      message(libs:debug)
      qtlibraries.files += \
      $$QTDIR_BIN/QtCored4.dll \
      $$QTDIR_BIN/QtGuid4.dll \
      $$QTDIR_BIN/QtSvgd4.dll \
      $$QTDIR_BIN/QtXmld4.dll \
#      $$QTDIR_BIN/QtNetworkd4.dll
  }
}

#
# Targets for debian source and binary package creation
#
debian-src.commands = dpkg-buildpackage -S -r -us -uc -d
debian-bin.commands = dpkg-buildpackage -b -r -uc -d
debian-all.depends = debian-src debian-bin


OTHER_FILES += \
    debian/changelog \
    debian/rules \
    debian/control \
    README \
    LICENSE.LGPL \
    TODO \
    src/words.txt \
    semaphored.spec
