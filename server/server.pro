TARGET = chatServer
TEMPLATE = app
CONFIG += c++17

QT += core network widgets concurrent sql
include(../common/common.pri)
INCLUDEPATH += ..

HEADERS += \
	DatabaseManager.h	\
	ServerModel.h		\
	ServerView.h		\
	ServerController.h	\
	TcpServer.h		\
	ClientHandler.h
	
SOURCES += \
	DatabaseManager.cpp	\
	ServerModel.cpp		\
	ServerView.cpp		\
	ServerController.cpp	\
	TcpServer.cpp		\
	ClientHandler.cpp	\
	mainServer.cpp
	
DESTDIR = $$OUT_PWD/../bin
CONFIG += file_copies
copies.files = $$PWD/../config/defaultConfig.json
copies.path = $$DESTDIR/config
QMAKE_EXTRA_TARGETS += copies
