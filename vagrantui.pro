#-------------------------------------------------
#
# Project created by QtCreator 2015-03-05T21:53:11
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vagrantui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    config.cpp \
    boxcreationdialog.cpp

HEADERS  += mainwindow.h \
    config.h \
    boxcreationdialog.h

FORMS    += mainwindow.ui \
    boxcreationdialog.ui

LIBS += -lqtermwidget5

OTHER_FILES += \
    app.rc

win32:RC_FILE = app.rc
