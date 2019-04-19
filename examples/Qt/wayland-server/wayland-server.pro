TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    compositor.c

LIBS += -L$$PWD/../install/lib/ -lwayland-server

INCLUDEPATH += $$PWD/../install/include
DEPENDPATH += $$PWD/../install/include
