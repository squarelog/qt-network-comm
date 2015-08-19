#-------------------------------------------------
#
# Project created by QtCreator 2013-05-14T07:42:42
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = embeddedsys-link
TEMPLATE = app


SOURCES += main.cpp\
    tcpworker.cpp \
    embeddedsys-link.cpp

HEADERS  += \
    tcpworker.h \
    telemetry_source.h \
    telemetrydata.h \
    telemetrydata.h \
    embeddedsys-link.h

FORMS    += embeddedsys-link.ui
