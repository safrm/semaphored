REM echo "windows: agent:%agent%"
REM call the qt
call C:\QtSDK\Desktop\Qt\4.7.4\mingw\bin\qtenv2.bat
IF NOT EXIST install mkdir install
set INSTALL_ROOT=%CD%\install
IF NOT EXIST output mkdir output
qmake 
mingw32-make release install
REM mingw32-make -f Makefile.Release install
"C:\Program Files\NSIS\makensis" semaphored.nsi
