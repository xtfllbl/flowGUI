#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "creatui.h"
#include "readxmlmodule.h"
#include "readxmlindex.h"
#include "qjdmodulelist.h"
#include "qjdflowlist.h"
#include "qjdstackwidget.h"
#include "creatjlt.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    qjdModuleList *moduleList;
    qjdStackWidget *stackWidget;

    qjdFlowList *flowList;
//    QPushButton *upButton;
//    QPushButton *downButton;
    QPushButton *delButton;

    creatJLT *jlt;

    QSplitter *splitter;
//    QSplitter splitter2;

    QFile fileXMLIndex;
    QFile fileXML;


private:
    Ui::MainWindow *ui;

    void resizeEvent(QResizeEvent *);

private slots:
    void on_actionExcute_triggered();
    void on_actionExit_triggered();
};

//internalWinId()
#endif // MAINWINDOW_H
