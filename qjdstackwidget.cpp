#include "qjdstackwidget.h"

// 在此widget中，拜访各种参数界面
qjdStackWidget::qjdStackWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    resize(300,300);
    hashCreat.clear();
    stackIndex=0;
//    hashIndex.clear();
    setFrameShape(QFrame::WinPanel);
    setFrameShadow(QFrame::Raised);
}

void qjdStackWidget::addFlowWidget(QString flowName, QString flowPath)
{
    qDebug()<<" qjdStackWidget::addFlowWidget";
    creatWidget(flowName,flowPath);
    /// 要先解析xml，再添加
//    addWidget()
}

void qjdStackWidget::creatWidget(QString /*flowName*/, const QString flowPath)
{
    qDebug()<<"qjdStackWidget::creatWidget";
    creat=new creatUI();   /// 一经创建，如何删除？？？
    xmlModule=new readXMLModule();

    hashCreat.insert(creat,stackIndex); //确保creat和index之间的关系不丢失
    stackIndex++;
    /// ---------------传递文件名--------------------------//
    if(!jobFileName.isEmpty())
    {
        creat->setJobFileName(jobFileName);
    }

    /// ----------------------------解析模块----------------------------
    /// 应当根据索引自动解析，而且是点击之后解析
    connect(xmlModule,SIGNAL(sigModuleName(QString)),creat,SLOT(getModuleName(QString)));
    connect(xmlModule,SIGNAL(sigNodeData(QString,QString,QString,QString,QString,QString,QString,QString,
                                         QStringList,QStringList,QString,QString)),
            creat,SLOT(nodeData(QString,QString,QString,QString,QString,QString,QString,QString,
                                QStringList,QStringList,QString,QString)));

    connect(xmlModule,SIGNAL(sigReadFinish()),creat,SLOT(finishCreatUI()));
    QFile fileXML;
    fileXML.setFileName(flowPath);
    if(!fileXML.open(QFile::ReadOnly))
        qDebug()<<"module file open failed at:"<<flowPath;

    xmlModule->read(&fileXML);
    fileXML.close();
    /// ------------------------------------------------------------------


    QScrollArea *area=new QScrollArea;
    area->setWidget(creat);
    addWidget(area);
    qDebug()<<"index of creat::"<<this->indexOf(creat);
    creat->setIndex(this->indexOf(creat));
    setCurrentWidget(area);  //变成需要设置area了..

    qDebug()<<"qjdStackWidget::creatWidget  out\n";
}

void qjdStackWidget::getJobXMLfileName(const QString fileName)
{
    qDebug()<<"module xml fileName arrived";
    jobFileName=fileName;
}

void qjdStackWidget::delFlowWidget(int stackIndex)
{
    qDebug()<<"delFlowWidget";
    creatUI *delCreat=hashCreat.key(stackIndex);  // 找出那个creat
    delCreat->deleteJob();

    delete delCreat;  // 删除本尊,但其实area还是存在的,所以不会造成崩溃
}

//void qjdStackWidget::moveUpFlowWidget(int stackIndex)
//{
//     //如何找到然后上下？？？
//    creatUI *upCreat=hashCreat.key(stackIndex);
//    upCreat->upJob();

//}
//void qjdStackWidget::moveDownFlowWidget(int stackIndex)
//{
//    creatUI *downCreat=hashCreat.key(stackIndex);
//    downCreat->downJob();
//}

void qjdStackWidget::turnOnWidget(int stackIndex)
{
    qDebug()<<"turn on";
    creatUI *onCreat=hashCreat.key(stackIndex);
    onCreat->turnOnJob();
}

void qjdStackWidget::turnOffWidget(int stackIndex)
{
    qDebug()<<"turn off";
    creatUI *offCreat=hashCreat.key(stackIndex);
    offCreat->turnOffJob();
}

void qjdStackWidget::dragWidget(int stackIndex, int currentIndex,int allRow)
{
    qDebug()<<"dragWidget";
    creatUI *dragCreat=hashCreat.key(stackIndex);

    if(stackIndex-currentIndex>0)
    {
        //up
        dragCreat->dragJob(currentIndex,allRow,"up");
    }
    if(stackIndex-currentIndex<0)
    {
        //down
        dragCreat->dragJob(currentIndex,allRow,"down");
    }
}

