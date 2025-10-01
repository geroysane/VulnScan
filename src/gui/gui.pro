TEMPLATE = app
TARGET = vulnscan_gui

include(../../common.pri)

QT += widgets

HEADERS += \
    MainWindow.h

SOURCES += \
    main.cpp \
    MainWindow.cpp

# Link to core library
LIBS += -L$$DESTDIR -lvulnscan_core
