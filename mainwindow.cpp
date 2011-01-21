#include "mainwindow.h"
#include <QDebug>
#include "ui_mainwindow.h"
#include <QLabel>
#include "signal.h"
#include "errno.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    //    setAttribute(Qt::WA_TranslucentBackground, true);
    setMaximumSize(1400,1000);
    QWidget *myWidget=new QWidget(this);
    this->setCentralWidget(myWidget);  //用来代替cw, 不知可行否

    QGridLayout *allGridLayout=new QGridLayout(myWidget);  /// 总的layout

    QSplitter *splitter=new QSplitter(myWidget);  /// 总的splitter
    splitter->setOrientation(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);

    QWidget *moduleLayoutWidget=new QWidget(splitter);  // 每个模块的那个小widget
    QWidget *flowLayoutWidget=new QWidget(splitter);
    QWidget *stackLayoutWidget=new QWidget(splitter);

    moduleList=new QJDModuleList(moduleLayoutWidget);
    flowList=new QJDFlowList(flowLayoutWidget);
    stackWidget=new QJDStackWidget(stackLayoutWidget);
    /// 这设定需要一定的改动,设置了无用，会被Splitter 关闭
    stackWidget->setMinimumWidth(400);
    moduleList->setMinimumWidth(100);
    flowList->setMinimumWidth(100);
    stackWidget->setMinimumHeight(500);
    moduleList->setMinimumHeight(500);
    flowList->setMinimumHeight(500);

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

    allGridLayout->addWidget(splitter, 0, 0, 1, 1);  //最终放进总的layout

    processWidget=new QJDProcessWidget();
    processWidget->setMaximumHeight(1);  //留一条缝隙
    processWidget->horizontalHeader()->setVisible(true);
    processWidget->verticalHeader()->setVisible(false);
    processWidget->horizontalHeader()->setStretchLastSection(true);
    allGridLayout->addWidget(processWidget,1,0,1,1);
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
    connect(flowList,SIGNAL(sigAddFlowWidget(QString,QString)),stackWidget,SLOT(addFlowWidget(QString,QString)));
    /// 依据hashItem来改变
    connect(flowList,SIGNAL(sigChangeStackWidgetIndex(int)),stackWidget,SLOT(setCurrentIndex(int)));

    // processWidget
    connect(processWidget,SIGNAL(pressed(QModelIndex)),this,SLOT(processWidgetPressed(QModelIndex)));

    /// ------------------------------------进程--------------------------------///

    connect(processWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showProcessContextMenu(const QPoint&)));

    processID=0;
    selectRow=0;

    processMenu = new QMenu(processWidget); /// 设置processWidget的右键菜单
    actStop = processMenu->addAction("Stop");
    actCon = processMenu->addAction("Continue");
    actTer = processMenu->addAction("Terminate");
    actKill = processMenu->addAction("Kill");
    actHan = processMenu->addAction("Hang Up");
    connect(actTer, SIGNAL(triggered()), this, SLOT(terProcess()));
    connect(actKill, SIGNAL(triggered()), this, SLOT(killProcess()));
    connect(actHan, SIGNAL(triggered()), this, SLOT(hanProcess()));
    connect(actStop, SIGNAL(triggered()), this, SLOT(stopProcess()));
    connect(actCon, SIGNAL(triggered()), this, SLOT(conProcess()));

    alreadyExpand=false;
    alreadyMini=false;
    expandTimer = new QTimer(this);
    miniTimer = new QTimer(this);
    connect(expandTimer, SIGNAL(timeout()), this, SLOT(large()));
    connect(miniTimer, SIGNAL(timeout()), this, SLOT(small()));

    proc=new Proc();
    proc->refresh();
    setProcData();

    refreshTimer = new QTimer(this);
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(autoRefresh()));

    /// 至少在点过一次excute之后启动
    refreshTimer->start(3000);
}

MainWindow::~MainWindow()
{}

void MainWindow::resizeEvent(QResizeEvent *)
{
    //    qDebug()<<"main size::"<<this->size();
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::on_actionExcute_triggered()
{
    jlt->startRun();
}

void MainWindow::on_actionExpand_triggered()
{
    expandTimer->start(50);
}

void MainWindow::on_actionMini_triggered()
{
    miniTimer->start(50);
}

void MainWindow::large()
{
    processWidget->setMaximumSize(33333,33333);
    this->setMaximumSize(33333,33333);

    if(processWidget->height()>=120 && processWidget->height()<160)
    {
        processWidget->setFixedHeight(processWidget->height()+2);
        setFixedHeight(height()+2);
    }
    else if(processWidget->height()>=160)
    {
        expandTimer->stop();
        processWidget->setMaximumSize(33333,33333);
        this->setMaximumSize(33333,33333);
    }
    else
    {
        processWidget->setFixedHeight(processWidget->height()+10);
        setFixedHeight(height()+10);
    }

    update();
}

void MainWindow::small()
{
    if(processWidget->height()>2 && processWidget->height()<30)
    {
        processWidget->setFixedHeight(processWidget->height()-2);
        setFixedHeight(height()-2);  // 这样不能拖拽了
    }
    else if(processWidget->height()<=2)
    {
        processWidget->setMaximumSize(33333,33333);
        this->setMaximumSize(33333,33333);
        miniTimer->stop();
    }
    else
    {
        processWidget->setFixedHeight(processWidget->height()-10);
        setFixedHeight(height()-10);
    }
    update();
}

void MainWindow::setProcData()
{
    model = new QStandardItemModel(0, 7, this);

    model->setHeaderData(0, Qt::Horizontal, tr("pid"));
    model->setHeaderData(1, Qt::Horizontal, tr("process name"));
//    model->setHeaderData(2, Qt::Horizontal, tr("state"));
//    model->setHeaderData(2, Qt::Horizontal, tr("nice"));
    model->setHeaderData(2, Qt::Horizontal, tr("starttime"));
//    model->setHeaderData(5, Qt::Horizontal, tr("wchan"));//
//    model->setHeaderData(4, Qt::Horizontal, tr("cpu"));
    model->setHeaderData(3, Qt::Horizontal, tr("Memory"));
//    model->setHeaderData(8, Qt::Horizontal, tr("memory(%)"));
//    model->setHeaderData(9, Qt::Horizontal, tr("sleep(%)"));
//    model->setHeaderData(10, Qt::Horizontal, tr("stack(K)"));
//    model->setHeaderData(6, Qt::Horizontal, tr("IO_READ"));
//    model->setHeaderData(12, Qt::Horizontal, tr("IO_WRITE"));
    model->setHeaderData(4, Qt::Horizontal, tr("% CPU"));
//    model->setHeaderData(14, Qt::Horizontal, tr("% WCPU"));
    model->setHeaderData(5, Qt::Horizontal, tr("COMMAND LINE"));
//    model->setHeaderData(16, Qt::Horizontal, tr("UID"));
    model->setHeaderData(6, Qt::Horizontal, tr("USERNAME"));

    // 向表格中输入数据
    /// 在此筛选,重要循环
    int countRow=0;
    for(int i=0;i<proc->pidVector.size();i++)
    {
        aCmd=proc->cmdVector.at(i);     // 通过cmd对过滤器作出反应
        if(aCmd=="gcalctool")
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
//            itemStat=new QStandardItem(aStat);
//            itemNice=new QStandardItem(aNice);
            itemStartTime=new QStandardItem(aStartTime);
//            itemWchan=new QStandardItem(aWchan);
//            itemWhichCpu=new QStandardItem(aWhichCpu);
            itemMem=new QStandardItem(aMem);
//            itemPmem=new QStandardItem(aPmem);
//            itemSleepAvg=new QStandardItem(aSleepAvg);
//            itemStack=new QStandardItem(aStack);
//            itemIoread=new QStandardItem(aIoread);
//            itemIowrite=new QStandardItem(aIowrite);
            itemPcpu=new QStandardItem(aPcpu);
//            itemWcpu=new QStandardItem(aWcpu);
            itemCmdLine=new QStandardItem(aCmdLine);
//            itemUid=new QStandardItem(aUid);
            itemUsrName=new QStandardItem(aUsrName);

            model->setItem(countRow,0,itemPid);
            model->setItem(countRow,1,itemCmd);
//            model->setItem(countRow,2,itemStat);
//            model->setItem(countRow,2,itemNice);
            model->setItem(countRow,2,itemStartTime);
//            model->setItem(countRow,5,itemWchan);
//            model->setItem(countRow,4,itemWhichCpu);
            model->setItem(countRow,3,itemMem);
//            model->setItem(countRow,8,itemPmem);
//            model->setItem(countRow,9,itemSleepAvg);
//            model->setItem(countRow,10,itemStack);
//            model->setItem(countRow,11,itemIoread);
//            model->setItem(countRow,12,itemIowrite);
            model->setItem(countRow,4,itemPcpu);
//            model->setItem(countRow,14,itemWcpu);
            model->setItem(countRow,5,itemCmdLine);
//            model->setItem(countRow,16,itemUid);
            model->setItem(countRow,6,itemUsrName);

            countRow++;
        }
    }
    if(countRow>0)
    {
        setExpand();
    }
    if(countRow==0)
    {
        if(processWidget->height()>5)
        {
            setMini();
        }
    }
    processWidget->setModel(model);

    processWidget->setCurrentIndex(processWidget->model()->index(selectRow,0));

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

void MainWindow::setExpand()
{
    if(alreadyExpand==false)
    {
        qDebug()<<"setEx````````````````````````````````";
        expandTimer->start(50);
        alreadyExpand=true;
        alreadyMini=false;
    }
}

void MainWindow::setMini()
{
    if(alreadyMini==false)
    {
        qDebug()<<"setMin```````````````````````````````";
        miniTimer->start(50);
        alreadyExpand=false;
        alreadyMini=true;
    }
}

void MainWindow::processWidgetPressed(QModelIndex index)
{
    index = processWidget->indexAt(processWidget->viewport()->mapFromGlobal(QCursor::pos()));
    selectRow=index.row();

    //获取所在行的pid
    processID=processWidget->model()->index(selectRow,0).data().toInt();
//    processName=ui->tblMain->model()->index(selectRow,1).data().toString();
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

void MainWindow::showProcessContextMenu(QPoint)
{
    if(processMenu)
    {
        processMenu->exec(QCursor::pos());
    }
}
