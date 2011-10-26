#include "mainwindow.h"
#include <QDebug>
#include "ui_mainwindow.h"
#include <QLabel>
#include "signal.h"
#include "errno.h"
#include "src/qjdargu.h"

// 还一处在creatjlts
#define PROGRAM "MainFlow"   // 过滤进程名称

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int width=QApplication::desktop()->width();         //获取屏幕的分辨率
    int height=QApplication::desktop()->height();
    this->move((width/2)-500,(height/2)-500);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    _image.load(":/images/window.png");
    setAutoFillBackground(true);

//    setMaximumSize(1400,1000);  // 直接导致不能最大化
    QWidget *myWidget=new QWidget(this);
    this->setCentralWidget(myWidget);  //用来代替cw, 不知可行否

    QGridLayout *allGridLayout=new QGridLayout(myWidget);  /// 总的layout

    splitter=new QSplitter(myWidget);  /// 上部的splitter
    splitter->setOrientation(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);

    splitter2=new QSplitter(myWidget);  /// 下部的splitter
    splitter2->setOrientation(Qt::Vertical);
    splitter2->setChildrenCollapsible(false);

    QWidget *moduleLayoutWidget=new QWidget(/*splitter*/);  // 每个模块的那个小widget
    QWidget *flowLayoutWidget=new QWidget(/*splitter*/);
    QWidget *stackLayoutWidget=new QWidget(/*splitter*/);

    moduleList=new QJDModuleList(moduleLayoutWidget);
    flowList=new QJDFlowList(flowLayoutWidget);
    stackWidget=new QJDStackWidget(stackLayoutWidget);
    /// 这设定需要一定的改动,设置了无用，会被Splitter 关闭
    stackWidget->setMinimumWidth(400);
    stackWidget->setMinimumHeight(250);
    moduleList->setMinimumWidth(200);
    moduleList->setMinimumHeight(250);
    flowList->setMinimumWidth(200);
    flowList->setMinimumHeight(250);

    alreadyExpand=false;
    alreadyMini=false;
    /// -------------------------------------module-----------------------------///
    QLabel *moduleTxt=new QLabel(moduleLayoutWidget);
    moduleTxt->setText("Module");
    moduleTxt->setFrameShape(QFrame::WinPanel);
    moduleTxt->setFrameShadow(QFrame::Raised);
    moduleTxt->setAlignment(Qt::AlignCenter);
    QVBoxLayout *moduleLayout=new QVBoxLayout(moduleLayoutWidget);
    moduleLayout->addWidget(moduleTxt);
    moduleLayout->addWidget(moduleList);

    splitter->addWidget(moduleLayoutWidget);

    /// -------------------------------------flow-----------------------------///
    QLabel *flowTxt=new QLabel(flowLayoutWidget);
    flowTxt->setText("Flow");
    flowTxt->setFrameShape(QFrame::WinPanel);
    flowTxt->setFrameShadow(QFrame::Raised);
    flowTxt->setAlignment(Qt::AlignCenter);
    QVBoxLayout *flowLayout=new QVBoxLayout(flowLayoutWidget);
    flowLayout->addWidget(flowTxt);
    flowLayout->addWidget(flowList);

    splitter->addWidget(flowLayoutWidget);

    /// -------------------------------------stack-----------------------------///
    QLabel *stackTxt=new QLabel(stackLayoutWidget);
    stackTxt->setText("Argument");
    stackTxt->setFrameShape(QFrame::WinPanel);
    stackTxt->setFrameShadow(QFrame::Raised);
    stackTxt->setAlignment(Qt::AlignCenter);
    QVBoxLayout *arguLayout=new QVBoxLayout(stackLayoutWidget);
    arguLayout->addWidget(stackTxt);
    arguLayout->addWidget(stackWidget);

    splitter->addWidget(stackLayoutWidget);
    splitter2->addWidget(splitter);

//    allGridLayout->addWidget(splitter, 0, 0, 1, 1);  //最终放进总的layout

    /// -------------------------------------tabWidget-----------------------------///
    tabWidget = new QTabWidget(this);
    tabWidget->setTabPosition(QTabWidget::North);

    // -------------------------------------process-----------------------------//
    tabProcess = new QWidget();
    gLayoutTab = new QGridLayout(tabProcess);
    processWidget=new QJDProcessWidget(tabProcess);
    processWidget->horizontalHeader()->setVisible(true);
    processWidget->verticalHeader()->setVisible(false);
    processWidget->horizontalHeader()->setStretchLastSection(true);
    gLayoutTab->addWidget(processWidget, 0, 0, 1, 1);
    tabWidget->addTab(tabProcess, "Process");

    // -------------------------------------histroy-----------------------------//
    tabLog = new QWidget();
    gLayoutProcess = new QGridLayout(tabLog);
    historyBrowser=new QJDHistoryBrowser();
    historyBrowser->setMinimumHeight(150);
    historyBrowser->verticalHeader()->setVisible(false);
    historyBrowser->setMinimumWidth(200);
    historyBrowser->horizontalHeader()->setStretchLastSection(true);
    gLayoutProcess->addWidget(historyBrowser, 0, 0, 1, 1);
    tabWidget->addTab(tabLog, "Log");

    tabWidget->setTabIcon(0,QIcon(":/images/process.png"));
    tabWidget->setTabIcon(1,QIcon(":/images/log.png"));

    splitter2->addWidget(tabWidget);
    allGridLayout->addWidget(splitter2,1,0,1,1);

    /// -------------------------------------------------signal && slot ------------------------------------------///
    jlt=new creatJLT();  //无关界面的一个类
    // 传个文件名过来而已
    connect(flowList,SIGNAL(sigJobXMLfileName(QString)),stackWidget,SLOT(getJobXMLfileName(QString)));
    connect(flowList,SIGNAL(sigJobXMLfileName(QString)),jlt,SLOT(setXMLJobListFileName(QString)));
    flowList->creatJobXML();

    //    connect(upButton,SIGNAL(pressed()),flowList,SLOT(upFlow()));
    //    connect(downButton,SIGNAL(pressed()),flowList,SLOT(downFlow()));
    //    connect(delButton,SIGNAL(pressed()),flowList,SLOT(delFlow()));
    /// xml当中去删除,上移，下移，开启，关闭
    connect(flowList,SIGNAL(sigDeleteFlow(int)),stackWidget,SLOT(delFlowWidget(int)));
    //    connect(flowList,SIGNAL(sigUpFlow(int)),stackWidget,SLOT(moveUpFlowWidget(int)));
    //    connect(flowList,SIGNAL(sigDownFlow(int)),stackWidget,SLOT(moveDownFlowWidget(int)));
    connect(flowList,SIGNAL(sigTurnOn(int)),stackWidget,SLOT(turnOnWidget(int)));
    connect(flowList,SIGNAL(sigTurnOff(int)),stackWidget,SLOT(turnOffWidget(int)));
    /// 拖拽
    connect(flowList,SIGNAL(sigDrag(int,int,int)),stackWidget,SLOT(dragWidget(int,int,int)));

    /// 链接module, flow, stack
    // 添加 flow
    connect(moduleList,SIGNAL(sigAddFlowList(QString,QString)),flowList,SLOT(addFlow(QString,QString)));
    // 添加 stack
    connect(flowList,SIGNAL(sigAddFlowWidget(QString,QString,int)),stackWidget,SLOT(addFlowWidget(QString,QString,int)));
    /// 依据hashItem来改变
    connect(flowList,SIGNAL(sigChangeStackWidgetIndex(int)),stackWidget,SLOT(setCurrentIndex(int)));

    // processWidget
    connect(processWidget,SIGNAL(pressed(QModelIndex)),this,SLOT(processWidgetPressed(QModelIndex)));
    connect(processWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(processWidgetDoubleClicked(QModelIndex)));
    // historyBrowser
    connect(historyBrowser,SIGNAL(showLog(QString)),this,SLOT(historyBrowserDoubleClicked(QString)));

    /// ------------------------------------进程--------------------------------///

    connect(processWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showProcessContextMenu(const QPoint&)));
    connect(historyBrowser, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showHistoryContextMenu(const QPoint&)));
    connect(historyBrowser,SIGNAL(cellPressed(int,int)),this,SLOT(saveRow(int,int)));  // 这里保存row?

    processID=0;
    selectRow=0;

    processMenu = new QMenu(processWidget); /// 设置processWidget的右键菜单
    actStop = processMenu->addAction("Stop");
    actStop->setIcon(QIcon(":/images/media-playback-stop.png"));
    actCon = processMenu->addAction("Continue");
    actCon->setIcon(QIcon(":/images/media-playback-start.png"));
    actTer = processMenu->addAction("Terminate");
    actTer->setIcon(QIcon(":/images/media-record.png"));
    actKill = processMenu->addAction("Kill");
    actKill->setIcon(QIcon(":/images/gtk-cancel.png"));
    actHan = processMenu->addAction("Hang Up");
    actHan->setIcon(QIcon(":/images/media-playback-pause.png"));
    connect(actTer, SIGNAL(triggered()), this, SLOT(terProcess()));
    connect(actKill, SIGNAL(triggered()), this, SLOT(killProcess()));
    connect(actHan, SIGNAL(triggered()), this, SLOT(hanProcess()));
    connect(actStop, SIGNAL(triggered()), this, SLOT(stopProcess()));
    connect(actCon, SIGNAL(triggered()), this, SLOT(conProcess()));

    logMenu = new QMenu(historyBrowser); /// 设置processWidget的右键菜单
    actShowLog = logMenu->addAction("Show Log");
    actShowLog->setIcon(QIcon(":/images/log.png"));
    connect(actShowLog, SIGNAL(triggered()), this, SLOT(showHistoryLog()));



    proc=new Proc();
    proc->refresh();
    setProcData();

    refreshTimer = new QTimer(this);
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(autoRefresh()));

    /// 至少在点过一次excute之后启动
    refreshTimer->start(3000);

    textBrowser=new QJDTextBrowser();

    /// -----------设置样式----------------- ///
    moduleTxt->setStyleSheet("border-image: url(:/styles/toolbarButton_hover.png) 3 3 3 3 stretch;"
                             "border-width: 3px 3px 3px 3px;"
                             "border-radius: 0px;");
    flowTxt->setStyleSheet("border-image: url(:/styles/toolbarButton_hover.png) 3 3 3 3 stretch;"
                           "border-width: 3px 3px 3px 3px;"
                           "border-radius: 0px;");
    stackTxt->setStyleSheet("border-image: url(:/styles/toolbarButton_hover.png) 3 3 3 3 stretch;"
                            "border-width: 3px 3px 3px 3px;"
                            "border-radius: 0px;");
    splitter->setStyleSheet("    QSplitter::handle {image: url(:/styles/normalButton_v.png);}QSplitter::handle:horizontal {width: 80px;}QSplitter::handle:vertical {height: 10px;}");
    splitter2->setStyleSheet("    QSplitter::handle {image: url(:/styles/normalButton.png);}QSplitter::handle:horizontal {width: 80px;}QSplitter::handle:vertical {height: 10px;}");

    QFile file(":/styles/stylesheet.qss");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString style = in.readAll();
    this->setStyleSheet(style);
}

MainWindow::~MainWindow()
{}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    QPalette pal(palette());
    pal.setBrush(QPalette::Window,
                QBrush(_image.scaled(event->size(), Qt::IgnoreAspectRatio,
                        Qt::SmoothTransformation)));
    setPalette(pal);
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::on_actionExcute_triggered()
{
    jlt->startRun();
}

void MainWindow::setProcData()
{
    model = new QStandardItemModel(0, 7, this);

    model->setHeaderData(0, Qt::Horizontal, tr("pid"));
    model->setHeaderData(1, Qt::Horizontal, tr("process name"));
    model->setHeaderData(2, Qt::Horizontal, tr("starttime"));
    model->setHeaderData(3, Qt::Horizontal, tr("Memory"));
    model->setHeaderData(4, Qt::Horizontal, tr("% CPU"));
    model->setHeaderData(5, Qt::Horizontal, tr("COMMAND LINE"));
    model->setHeaderData(6, Qt::Horizontal, tr("USERNAME"));

    QJDArgu *argu=new QJDArgu;
    // 向表格中输入数据
    /// 在此筛选,重要循环
    int countRow=0;
    for(int i=0;i<proc->pidVector.size();i++)
    {
        aCmd=proc->cmdVector.at(i);     // 通过cmd对过滤器作出反应
        if(aCmd==argu->FlowProcess)
        {
            // go on
            aPid=QString::number(proc->pidVector.at(i),10);
            if(proc->statVector.at(i)=="S")
            {
                aStat="Sleep";
            }
            if(proc->statVector.at(i)=="R")
            {
                aStat="Running";
            }
            if(proc->statVector.at(i)=="Z")
            {
                aStat="Zombie";
            }
            if(proc->statVector.at(i)=="T")
            {
                aStat="Stoped";
            }
            if(proc->statVector.at(i)=="D")
            {
                aStat="Uninterruptible";
            }
            aNice=QString::number(proc->niceVector.at(i),10);
            aStartTime=proc->starttimeVector.at(i);
            aWchan=proc->wchanVector.at(i);
            aWhichCpu=QString::number(proc->whichcpuVector.at(i),10);
            aMem=proc->memVector.at(i);
            aPmem.sprintf("%f",proc->pmemVector.at(i));
            aSleepAvg=QString::number(proc->slpavgVector.at(i),10);
            aStack=QString::number(proc->stackVector.at(i),10);
            aIoread=proc->ioreadVector.at(i);
            aIowrite=proc->iowriteVector.at(i);
            aPcpu=QString::number(proc->pcpuVector.at(i),10);
            aWcpu.sprintf("%f",proc->wcpuVector.at(i));
            aCmdLine=proc->cmdlineVector.at(i);
            aUid=QString::number(proc->uidVector.at(i),10);
            aUsrName=proc->usernameVector.at(i);

            itemPid=new QStandardItem(aPid);
            itemCmd=new QStandardItem(aCmd);
            itemStartTime=new QStandardItem(aStartTime);
            itemMem=new QStandardItem(aMem);
            itemPcpu=new QStandardItem(aPcpu);
            itemCmdLine=new QStandardItem(aCmdLine);
            itemUsrName=new QStandardItem(aUsrName);

            model->setItem(countRow,0,itemPid);
            model->setItem(countRow,1,itemCmd);
            model->setItem(countRow,2,itemStartTime);
            model->setItem(countRow,3,itemMem);
            model->setItem(countRow,4,itemPcpu);
            model->setItem(countRow,5,itemCmdLine);
            model->setItem(countRow,6,itemUsrName);

            /// 如何保存model, 或者保存item?
            countRow++;
        }
    }

    processWidget->setModel(model);
    processWidget->setCurrentIndex(processWidget->model()->index(selectRow,0));
    processWidget->horizontalHeader()->setStretchLastSection(true);   //不够就不调整了？？


    saveHash=jlt->getJobHash();
//    qDebug()<<"saveHash"<<saveHash;  /// 双击之后，依据pid，给出jobX.log
}



void MainWindow::autoRefresh()
{
    vectorClear();
    proc->refresh();
    setProcData();
}

void MainWindow::vectorClear()
{
    delete model;       //减少一大半的内存泄露

    proc->pidVector.clear();            //pid
    proc->cmdVector.clear();        //进程名
    proc->statVector.clear();        // 状态
    proc->niceVector.clear();
    proc->starttimeVector.clear();
    proc->wchanVector.clear();       //与下文wchan_str相匹配
    proc->whichcpuVector.clear();
    proc->memVector.clear();       //内存使用量
    proc->pmemVector.clear();                    //内存使用百分比
    proc->slpavgVector.clear();       //睡眠百分比
    proc->stackVector.clear();       //栈空间
    proc->ioreadVector.clear();
    proc->iowriteVector.clear();
    proc->pcpuVector.clear();                    //cpu使用百分比
    proc->wcpuVector.clear();                    //cpu使用30s内百分比
    proc->cmdlineVector.clear();
    proc->uidVector.clear();
    proc->usernameVector.clear();

    proc->originIoreadVector.clear();
    proc->originIowriteVector.clear();
    proc->originMemVector.clear();
    proc->originStarttimeVector.clear();
}

void MainWindow::processWidgetPressed(QModelIndex index)
{
    qDebug()<<"processWidgetPressed  ~~~";
    index = processWidget->indexAt(processWidget->viewport()->mapFromGlobal(QCursor::pos()));
    selectRow=index.row();

    //获取所在行的pid
    processID=processWidget->model()->index(selectRow,0).data().toInt();
//    processName=ui->tblMain->model()->index(selectRow,1).data().toString();
}

void MainWindow::processWidgetDoubleClicked(QModelIndex /*index*/)
{
    qDebug()<<"processWidgetDoubleClicked";   //会先触动press，再触动这里,所以已经有了selectRow,processID
    qDebug()<<"job ::"<<saveHash.value(processID);  //返回了jobX, 然后调用显示就是了
    if(textBrowser->isVisible()==false)
    {
        textBrowser->show();
    }
    QString fileName;
    fileName.append("log/job");
    fileName.append(QString::number(saveHash.value(processID)+1));
    fileName.append(".log");
    textBrowser->openLog(fileName);
}

void MainWindow::historyBrowserDoubleClicked(QString fileName)
{
    textBrowser->openLog(fileName);
    if(textBrowser->isVisible()==false)
    {
        textBrowser->show();
    }
    textBrowser->raise();  //会造成第二次之后无法显示到后台
}

void MainWindow::killProcess()
{
    send_to_selected(SIGKILL);
}

void MainWindow::terProcess()
{
    send_to_selected(SIGTERM);
}

void MainWindow::hanProcess()
{
    send_to_selected(SIGHUP);
}
void MainWindow::stopProcess()
{
    send_to_selected(SIGSTOP);
}
void MainWindow::conProcess()
{
    send_to_selected(SIGCONT);
}

void MainWindow::send_to_selected(int sig)
{
    sendsig(processID, sig);
}

void MainWindow::sendsig(int pid, int sig)
{
    if(kill(pid, sig) < 0)
    {
        if(errno == EPERM)
        {
            QString s;
            s.sprintf("You do not have permission to send a signal to"
                      " process %d (", processID);
            s.append(").\n"
                     "Only the super-user and the owner of the process"
                     " may send signals to it.");
            QMessageBox::warning(this, "Permission denied", s);
        }
    }
}
void MainWindow::saveRow(int row, int)
{
    clickedRow=row;
}
void MainWindow::showHistoryLog()
{
    qDebug()<<"showHistoryLog";
    QString fileName;
    fileName=historyBrowser->hashFilePath.value(clickedRow);  //需要行号0.1.2.3
    textBrowser->openLog(fileName);  ///  这个文件路径哪里来
    if(textBrowser->isVisible()==false)
    {
        textBrowser->show();
    }
    textBrowser->raise();
}
void MainWindow::showProcessContextMenu(QPoint)
{
    if(processMenu)
    {
        processMenu->exec(QCursor::pos());
    }
}
void MainWindow::showHistoryContextMenu(QPoint)
{
    if(logMenu)
    {
        logMenu->exec(QCursor::pos());
    }
}
