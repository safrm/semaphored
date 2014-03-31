include(../common.pri)
CONFIG      -= debug

QMAKE_LINK = @: IGNORE THIS LINE
QMAKE_EXTRA_TARGETS += doc
QMAKE_EXTRA_COMPILER += doc

doc.target        = htmlpages/semaphored.html
doc.CONFIG       += no_check_exist no_link explicit_dependencies disable_check

packagesExist(jenkins-support-scripts) {
    doc.commands = @echo "Not installed jenkins-support-scripts"
} else {
    doc.commands      = jss-docs-update . -sv $$APP_FULL_VERSION_TAG
}
doc.input         = $${doc.depends}
doc.output        = $${doc.target}

## Make are doc target build by default If 'make' is executed in doc folder
PRE_TARGETDEPS += $${doc.output}


	INSTALLS    += readme
	unix:readme.path  = /usr/share/doc/semaphored
	win32:readme.path  = /
	readme.files  = ../README
	unix:readme.commands = sed -i".bkp" \'1 s/Version:.*/Version: $$APP_FULL_VERSION_TAG ($$APP_BUILD_DATE)/\' ../README && rm -f ../README.bkp

    INSTALLS    += manpage
	win32:manpage.path  = /
	unix:manpage.path = /usr/share/man/man1/
	manpage.files  = manpages/semaphored.1
	manpage.CONFIG += no_check_exist disable_check

win32 {
	INSTALLS    += htmldocs
	win32:htmldoc.path  = /
	unix:htmldocs.path = /usr/share/doc/semaphored/html/
	htmldocs.files  = htmlpages/*.html
	htmldocs.CONFIG += no_check_exist disable_check

    INSTALLS    += docs
    unix:docs.path  = /usr/share/doc/semaphored
    win32:docs.path  = /
    docs.files  = ../LICENSE.LGPL

}

OTHER_FILES += \
    semaphored.1.xml \
    ../LICENSE.LGPL \
    ../README

