TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
SOURCES += main.c

LIBS += -L$$PWD/../install/lib/ -lwayland-client

INCLUDEPATH += $$PWD/../install/include
DEPENDPATH += $$PWD/../install/include
