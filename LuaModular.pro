TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG(release, debug|release): LIBS += -ldl
CONFIG(debug, debug|release): LIBS += -ldl
CONFIG(release, debug|release): LIBS += -lluajit-5.1
CONFIG(debug, debug|release): LIBS += -lluajit-5.1
INCLUDEPATH += "/usr/include/luajit-2.0/"
DEPENDPATH += "/usr/include/luajit-2.0/"

SOURCES += main.c \
    module.c

HEADERS += \
    module.h
