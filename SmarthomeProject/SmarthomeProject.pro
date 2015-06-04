#-------------------------------------------------
#
# Project created by QtCreator 2015-05-18T13:26:15
#
#-------------------------------------------------

QT       += core gui network sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmarthomeProject
TEMPLATE = app


SOURCES += main.cpp \
    smartbusfuntion.cpp \
    SmartBus.cpp \
    mysql.cpp \
    myserver.cpp \
    arduPi.cpp \
    uartthread.cpp \
    tcpthread.cpp

HEADERS  += mainwindow.h \
    smartbusfuntion.h \
    SmartBus.h \
    mysql.h \
    myserver.h \
    arduPi.h \
    uartthread.h \
    tcpthread.h

FORMS    +=


LIBS+= -L$$OUT_PWD/../../../SmarthomeProject/ -lpthread -lrt
INCLUDEPATH+= $$PWD/../../../SmarthomeProject
