#-------------------------------------------------
#
# Project created by QtCreator 2010-06-29T13:01:17
#
#-------------------------------------------------
# 解析模块xml, 窗件界面
#TODO: 可隐藏界面的制作

QT       += core gui
QT += xml

TARGET = flowGui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    creatui.cpp \
    readxmlmodule.cpp \
    readxmlindex.cpp \
    qjdflowlist.cpp \
    qjdmodulelist.cpp \
    qjdstackwidget.cpp \
    writejob.cpp \
    creatjlt.cpp \
    src/qjdlineedit.cpp \
    src/qjdradiobutton.cpp \
    src/qjdcheckbox.cpp \
    src/qjdcombobox.cpp \
    src/qjdspinbox.cpp \
    src/qjdgroupbox.cpp \
    src/qjdprocess.cpp \
    qjdprocesswidget.cpp \
    process/qjdproc.cpp \
    process/qjdmisc.cpp

HEADERS  += mainwindow.h \
    creatui.h \
    readxmlmodule.h \
    readxmlindex.h \
    qjdflowlist.h \
    qjdmodulelist.h \
    qjdstackwidget.h \
    writejob.h \
    creatjlt.h \
    src/qjdlineedit.h \
    src/qjdradiobutton.h \
    src/qjdcheckbox.h \
    src/qjdcombobox.h \
    src/qjdspinbox.h \
    src/qjdgroupbox.h \
    src/qjdprocess.h \
    qjdprocesswidget.h \
    process/qjdproc.h \
    process/config.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    images.qrc
