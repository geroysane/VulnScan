TEMPLATE = app
TARGET = vulnscan_cli

include(../../common.pri)

QT -= gui widgets

HEADERS += \
    CommandLineParser.h

SOURCES += \
    main.cpp \
    CommandLineParser.cpp

# Link to core library
LIBS += -L$$DESTDIR -lvulnscan_core
