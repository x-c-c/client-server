INCLUDEPATH += $$PWD

COMMON_HEADERS = \
	$$PWD/ConfigManager.h\
	$$PWD/ConfigKeys.h

COMMON_SOURCES = \
	$$PWD/ConfigManager.cpp

SOURCES += $$COMMON_SOURCES
HEADERS += $$COMMON_HEADERS

Qt += core
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

