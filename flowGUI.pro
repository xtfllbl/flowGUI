QT       += core gui
QT += xml

TARGET = flowGui
TEMPLATE = app

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/moc
OBJECTS_DIR += ./GeneratedFiles/ofile
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

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
    process/qjdmisc.cpp \
    src/qjdtextbrowser.cpp \
    src/qjdhighlighter.cpp \
    src/qjdfilecombobox.cpp \
    qjdhistorybrowser.cpp \
    src/qjdargu.cpp

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
    process/config.h \
    src/qjdtextbrowser.h \
    src/qjdhighlighter.h \
    src/qjdfilecombobox.h \
    qjdhistorybrowser.h \
    src/qjdargu.h

FORMS    += \
    mainwindow.ui \
    src/qjdtextbrowser.ui

RESOURCES += \
    images.qrc \
    stylesheet.qrc \
    xml.qrc

OTHER_FILES += \
    styles/stylesheet.qss


