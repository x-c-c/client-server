TARGET = chatClient
TEMPLATE = app
CONFIG += c++17

QT += core network widgets

include(../common/common.pri)

HEADERS += \
	ClientModel.h		\
	ClientView.h		\
	ClientController.h

SOURCES += \
	ClientModel.cpp		\
	ClientView.cpp		\
	ClientController.cpp	\
	mainClient.cpp
	
DESTDIR = $$OUT_PWD/../bin

CONFIG += file_copies
copies.files = $$PWD/../config/defaultConfig.json
copies.path = $$DESTDIR/config
QMAKE_EXTRA_TARGETS += copies
