QT = core
QT += core network
CONFIG  += c++17 cmdline

SOURCES += \
	mainServer.cpp
	

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
