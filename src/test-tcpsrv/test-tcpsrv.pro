#-------------------------------------------------
#
# Project created by QtCreator 2013-05-14T07:42:42
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test-tcpsrv
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpworker.cpp

HEADERS  += mainwindow.h \
    tcpworker.h \
    telemetry_source.h \
    telemetrydata.h \
    telemetrydata.h

FORMS    += mainwindow.ui
