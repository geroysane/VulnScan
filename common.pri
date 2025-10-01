QT += core network sql
CONFIG += c++17 warn_on

# Compiler flags
gcc|clang {
    QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic
}

msvc {
    QMAKE_CXXFLAGS += /W4 /std:c++17
}

# Output directories
CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/build/debug
    OBJECTS_DIR = $$PWD/build/debug/.obj
    MOC_DIR = $$PWD/build/debug/.moc
} else {
    DESTDIR = $$PWD/build/release
    OBJECTS_DIR = $$PWD/build/release/.obj
    MOC_DIR = $$PWD/build/release/.moc
}

INCLUDEPATH += $$PWD/src
