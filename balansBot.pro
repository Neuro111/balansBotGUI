#-------------------------------------------------
#
# Project created by QtCreator 2016-05-19T08:26:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = balansBot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    communicationwidget.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    communicationwidget.h

FORMS    += mainwindow.ui \
    communicationwidget.ui
