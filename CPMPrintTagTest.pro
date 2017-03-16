#-------------------------------------------------
#
# Project created by QtCreator 2017-03-16T14:09:48
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPMPrintTagTest
TEMPLATE = app

LIBS += zint.dll

SOURCES += main.cpp\
        mainwindow.cpp \
    tagprinter.cpp

HEADERS  += mainwindow.h \
    tagprinter.h

FORMS    += mainwindow.ui
