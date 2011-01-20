#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QTimer>
#include "creatui.h"
#include "readxmlmodule.h"
#include "readxmlindex.h"
#include "qjdmodulelist.h"
#include "qjdflowlist.h"
#include "qjdstackwidget.h"
#include "qjdprocesswidget.h"
#include "creatjlt.h"
#include "process/qjdproc.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QJDModuleList *moduleList;
    QJDStackWidget *stackWidget;
    QJDFlowList *flowList;
    QJDProcessWidget *processWidget;
    creatJLT *jlt;

    Proc *proc;

    QSplitter *splitter;
//    QSplitter splitter2;

    QFile fileXMLIndex;
    QFile fileXML;


private:
    Ui::MainWindow *ui;

    void resizeEvent(QResizeEvent *);

    void setProcData();
//    QTableWidget *processWidget;
    QTimer *expandTimer;
    QTimer *miniTimer;
    QTimer *refreshTimer;

    QStandardItemModel *model;
    QMenu *processMenu;

    QString aPid;
    QString aCmd;
    QString aStat;
    QString aNice;
    QString aStartTime;
    QString aWchan;
    QString aWhichCpu;
    QString aMem;
    QString aPmem;
    QString aSleepAvg;
    QString aStack;
    QString aIoread;
    QString aIowrite;
    QString aPcpu;
    QString aWcpu;
    QString aCmdLine;  //cmdlineVector
    QString aUid;
    QString aUsrName;

    QStandardItem *itemPid;
    QStandardItem *itemCmd;
    QStandardItem *itemStat;
    QStandardItem *itemNice;
    QStandardItem *itemStartTime;
    QStandardItem *itemWchan;
    QStandardItem *itemWhichCpu;
    QStandardItem *itemMem;
    QStandardItem *itemPmem;
    QStandardItem *itemSleepAvg;
    QStandardItem *itemStack;
    QStandardItem *itemIoread;
    QStandardItem *itemIowrite;
    QStandardItem *itemPcpu;
    QStandardItem *itemWcpu;
    QStandardItem *itemCmdLine;
    QStandardItem *itemUid;
    QStandardItem *itemUsrName;

    bool alreadyExpand;
    bool alreadyMini;

    int processID;
    int selectRow;

    QMenu *menu;
    QAction *actStop;
    QAction *actCon;
    QAction *actTer;
    QAction *actKill;
    QAction *actHan;
    QAction *actView;

private slots:
    void on_actionMini_triggered();
    void on_actionExpand_triggered();
    void on_actionExcute_triggered();
    void on_actionExit_triggered();
    void processWidgetPressed(QModelIndex);

    void large();
    void small();

    void setExpand();
    void setMini();

    void autoRefresh();
    void vectorClear();

    void killProcess();
    void terProcess();
    void hanProcess();
    void stopProcess();
    void conProcess();
    void send_to_selected(int);
    void sendsig(int, int);

    void showProcessContextMenu(QPoint);

};

//internalWinId()
#endif // MAINWINDOW_H
