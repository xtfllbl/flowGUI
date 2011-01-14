#include "mainwindow.h"
#include <QDebug>
#include "ui_mainwindow.h"
#include <QLabel>

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

    moduleList=new qjdModuleList(moduleLayoutWidget);
    flowList=new qjdFlowList(flowLayoutWidget);
    stackWidget=new qjdStackWidget(stackLayoutWidget);
    /// 这设定需要一定的改动,设置了无用，会被Splitter 关闭
    stackWidget->setMinimumWidth(400);
    moduleList->setMinimumWidth(100);
    flowList->setMinimumWidth(100);

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
//    upButton=new QPushButton(flowLayoutWidget);
//    downButton=new QPushButton(flowLayoutWidget);
//    delButton=new QPushButton(flowLayoutWidget);
//    upButton->setText("UP");
//    downButton->setText("DOWN");
//    delButton->setText("DEL");
//    QVBoxLayout *vFlowLayout=new QVBoxLayout();
//    vFlowLayout->addWidget(upButton);
//    vFlowLayout->addWidget(downButton);
//    vFlowLayout->addWidget(delButton);
//    QSpacerItem *spaceItem=new QSpacerItem(0,100, QSizePolicy::Minimum,QSizePolicy::Expanding);
//    vFlowLayout->addSpacerItem(spaceItem);
//    QHBoxLayout *hFlowLayout=new QHBoxLayout();
//    hFlowLayout->addWidget(flowList);
//    hFlowLayout->addLayout(vFlowLayout);

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

//    this->set
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
