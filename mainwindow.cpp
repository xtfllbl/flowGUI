#include "mainwindow.h"
#include <QDebug>
#include "ui_mainwindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMaximumSize(1400,1000);
    QWidget *myWidget=new QWidget();

    moduleList=new qjdModuleList();
    stackWidget=new qjdStackWidget();
    flowList=new qjdFlowList();

    connect(flowList,SIGNAL(sigJobXMLfileName(QString)),stackWidget,SLOT(getJobXMLfileName(QString)));
    flowList->creatJobXML();


    QLabel *a=new QLabel;
    QLabel *b=new QLabel;
    QLabel *c=new QLabel;
    a->setText("Module");
    a->setFrameShape(QFrame::WinPanel);
    a->setFrameShadow(QFrame::Raised);
    a->setAlignment(Qt::AlignCenter);
    b->setText("Flow");
    b->setFrameShape(QFrame::WinPanel);
    b->setFrameShadow(QFrame::Raised);
    b->setAlignment(Qt::AlignCenter);
    c->setText("Argument");
    c->setFrameShape(QFrame::WinPanel);
    c->setFrameShadow(QFrame::Raised);
    c->setAlignment(Qt::AlignCenter);
    QHBoxLayout *hLayout=new QHBoxLayout();
    QVBoxLayout *moduleLayout=new QVBoxLayout();
    QVBoxLayout *flowLayout=new QVBoxLayout();
    QVBoxLayout *arguLayout=new QVBoxLayout();

    moduleLayout->addWidget(a);
    moduleLayout->addWidget(moduleList);
    flowLayout->addWidget(b);
    flowLayout->addWidget(flowList);
    arguLayout->addWidget(c);
    arguLayout->addWidget(stackWidget);
//    arguLayout->addWidget(area);
    hLayout->addLayout(moduleLayout);
    hLayout->addLayout(flowLayout);
    hLayout->addLayout(arguLayout);

    stackWidget->setMinimumWidth(400);
    moduleList->setMinimumWidth(200);
    setCentralWidget(myWidget);
    myWidget->setLayout(hLayout);

    connect(moduleList,SIGNAL(sigAddFlow(QString)),flowList,SLOT(addFlow(QString)));
    connect(moduleList,SIGNAL(sigAddFlowWidget(QString,QString)),stackWidget,SLOT(addFlowWidget(QString,QString)));

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
