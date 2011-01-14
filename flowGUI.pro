#-------------------------------------------------
#
# Project created by QtCreator 2010-06-29T13:01:17
#
#-------------------------------------------------
# 解析模块xml, 窗件界面
#TODO: 可隐藏界面的制作

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
    writejob.cpp \
    creatjlt.cpp \
    widget/qjdlineedit.cpp \
    widget/qjdradiobutton.cpp \
    widget/qjdcheckbox.cpp \
    widget/qjdcombobox.cpp \
    widget/qjdspinbox.cpp \
    widget/qjdgroupbox.cpp

HEADERS  += mainwindow.h \
    creatui.h \
    readxmlmodule.h \
    readxmlindex.h \
    qjdflowlist.h \
    qjdmodulelist.h \
    qjdstackwidget.h \
    writejob.h \
    creatjlt.h \
    widget/qjdlineedit.h \
    widget/qjdradiobutton.h \
    widget/qjdcheckbox.h \
    widget/qjdcombobox.h \
    widget/qjdspinbox.h \
    widget/qjdgroupbox.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    images.qrc
