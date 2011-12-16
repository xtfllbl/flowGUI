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
#include "src/qjdtextbrowser.h"
#include "qjdhistorybrowser.h"
#include "src/qjdargu.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QJDArgu *argu ;

    // 需要的控件
    QJDModuleList *moduleList;
    QJDStackWidget *stackWidget;
    QJDFlowList *flowList;
    creatJLT *jlt;
    QJDTextBrowser *textBrowser;
    Proc *proc;

    // tabView 下的
    QTabWidget *tabWidget;
    QGridLayout *gLayoutTab;
    QGridLayout *gLayoutProcess;
    QWidget *tabProcess;
    QJDProcessWidget *processWidget;
    QWidget *tabLog;
    QJDHistoryBrowser *historyBrowser;

    QSplitter *splitter;
    QSplitter *splitter2;

    QFile fileXMLIndex;
    QFile fileXML;


private:
    Ui::MainWindow *ui;

    void resizeEvent(QResizeEvent *);
    QImage _image;

    void setProcData();

//    QTableWidget *processWidget;
    QTimer *expandTimer;
    QTimer *miniTimer;
    QTimer *refreshTimer;

    QStandardItemModel *model;
    QMenu *processMenu;
    QMenu *logMenu;

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

    int clickedRow;
    QMenu *menu;
    QAction *actStop;
    QAction *actCon;
    QAction *actTer;
    QAction *actKill;
    QAction *actHan;
    QAction *actView;

    QAction *actShowLog;

    QHash<int,int> saveHash;

private slots:
    void on_actionExcute_triggered();
    void on_actionExit_triggered();
    void processWidgetPressed(QModelIndex);

    void autoRefresh();
    void vectorClear();

    void killProcess();
    void terProcess();
    void hanProcess();
    void stopProcess();
    void conProcess();
    void send_to_selected(int);
    void sendsig(int, int);
    void saveRow(int,int);
    void showHistoryLog();

    void showProcessContextMenu(QPoint);
    void showHistoryContextMenu(QPoint);
    void processWidgetDoubleClicked(QModelIndex);
    void historyBrowserDoubleClicked(QString fileName);

};

//internalWinId()
#endif // MAINWINDOW_H
