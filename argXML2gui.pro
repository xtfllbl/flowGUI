#-------------------------------------------------
#
# Project created by QtCreator 2010-06-29T13:01:17
#
#-------------------------------------------------
# 解析模块xml, 窗件界面

QT       += core gui
QT += xml

TARGET = argXML2gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    creatui.cpp \
    readxmlmodule.cpp \
    readxmlindex.cpp \
    qjdflowlist.cpp \
    qjdmodulelist.cpp \
    qjdstackwidget.cpp \
    writejob.cpp

HEADERS  += mainwindow.h \
    creatui.h \
    readxmlmodule.h \
    readxmlindex.h \
    qjdflowlist.h \
    qjdmodulelist.h \
    qjdstackwidget.h \
    writejob.h

FORMS    += \
    mainwindow.ui
