INCLUDEPATH += $$PWD

COMMON_HEADERS = \
    $$PWD/ConfigKeys.h		\
    $$PWD/Protocol.h		\
    $$PWD/LogLevel.h		\
    $$PWD/ConfigManager.h	\
    $$PWD/Logger.h		\
    $$PWD/InputValidator.h	\
    $$PWD/PacketHandler.h	\
    $$PWD/Serializer.h		\
    $$PWD/RateLimiter.h		\
    $$PWD/Argon2Hasher.h

COMMON_SOURCES = \
    $$PWD/ConfigManager.cpp	\
    $$PWD/Logger.cpp		\
    $$PWD/InputValidator.cpp	\
    $$PWD/PacketHandler.cpp	\
    $$PWD/Serializer.cpp	\
    $$PWD/RateLimiter.cpp	\
    $$PWD/Argon2Hasher.cpp

SOURCES += $$COMMON_SOURCES
HEADERS += $$COMMON_HEADERS

QT += core
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

LIBS += -lspdlog -largon2
