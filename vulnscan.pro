TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    src/core \
    src/gui \
    src/cli \
    tests

# Dependencies
src/gui.depends = src/core
src/cli.depends = src/core
tests.depends = src/core
