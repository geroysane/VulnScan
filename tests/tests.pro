TEMPLATE = app
TARGET = vulnscan_tests

include(../common.pri)

QT += testlib
QT -= gui widgets

CONFIG += testcase

HEADERS += \
    TestDatabaseManager.h \
    TestConfiguration.h

SOURCES += \
    main.cpp \
    TestDatabaseManager.cpp \
    TestConfiguration.cpp

# Link to core library
LIBS += -L$$DESTDIR -lvulnscan_core
