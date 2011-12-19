#include "qjdstackwidget.h"

// 在此widget中，拜访各种参数界面
QJDStackWidget::QJDStackWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    hashCreat.clear();
    stackIndex=0;
//    hashIndex.clear();
    setFrameShape(QFrame::WinPanel);
    setFrameShadow(QFrame::Raised);
}

void QJDStackWidget::addFlowWidget(QString flowName, QString flowPath,int value)
{
    qDebug()<<" qjdStackWidget::addFlowWidget"<<hashCreat;
    creatWidget(flowName,flowPath,value);
    qDebug()<<" qjdStackWidget::addFlowWidget"<<hashCreat;
}

void QJDStackWidget::addHashListAndCreat(QListWidgetItem *item)
{
    qDebug()<<"QJDStackWidget::addHashListAndCreat::"<<item<<creat;
    hashListCreat.insert(item,currentIndex());
}

void QJDStackWidget::creatWidget(QString /*flowName*/, const QString flowPath, int value)
{
    qDebug()<<"qjdStackWidget::creatWidget";
    creat=new creatUI();   /// 一经创建，如何删除？？？
    xmlModule=new readXMLModule();

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
    /// 这个Hash同样需要改变,需要和hashItem保持相同的改变顺序
//    qDebug()<<"value::"<<value<<"stackIndex::"<<stackIndex;
    if(value==stackIndex)
    {
        hashCreat.insert(creat,stackIndex); //确保creat和index之间的关系不丢失
    }
    if(value<stackIndex)
    {
        insertHash(value,creat); // 插入现有之后使用这个调整
        creat->resetJob(stackIndex-value);
    }
    stackIndex++;
    /// ------------------------------------------------------------------
    qDebug()<<hashCreat;

    QScrollArea *area=new QScrollArea;
    area->setWidget(creat);
    addWidget(area);
//    qDebug()<<"index of creat::"<<this->indexOf(creat);
    creat->setIndex(this->indexOf(creat));
    setCurrentWidget(area);  //变成需要设置area了..

    qDebug()<<"qjdStackWidget::creatWidget  out\n";
}

void QJDStackWidget::insertHash(int value, creatUI *saveCreat)
{
    creatUI *creatui;
    foreach(creatui,hashCreat.keys())
    {
        if(creatui!=saveCreat && hashCreat.value(creatui)>=value)
        {
            hashCreat.insert(creatui,hashCreat.value(creatui)+1);
        }
    }
    hashCreat.insert(saveCreat,value);
}

void QJDStackWidget::getJobXMLfileName(const QString fileName)
{
//    qDebug()<<"module xml fileName arrived";
    jobFileName=fileName;
}

void QJDStackWidget::delFlowWidget(int stackIndex,int allIndex)
{
    creatUI *delCreat=hashCreat.key(stackIndex);  // 找出那个creat
    qDebug()<<"QJDStackWidget::delFlowWidget::"<<delCreat;
    delCreat->deleteJob();  /// 问题出在这
    resetDelHash(stackIndex,allIndex,delCreat);
    delete delCreat;  // 删除本尊,但其实area还是存在的,所以不会造成崩溃
}

void QJDStackWidget::turnOnWidget(int stackIndex)
{
    qDebug()<<"turn on";
    creatUI *onCreat=hashCreat.key(stackIndex);
    onCreat->turnOnJob();
}

void QJDStackWidget::turnOffWidget(int stackIndex)
{
    qDebug()<<"turn off";
    creatUI *offCreat=hashCreat.key(stackIndex);
    offCreat->turnOffJob();
}

void QJDStackWidget::dragWidget(int stackIndex, int currentIndex,int allRow)
{
    qDebug()<<"1.dragWidget";
    // 取出正确的dragCreat
    creatUI *dragCreat=hashCreat.key(stackIndex);  /// 这个是依据stackIndex的,所以不能改阿
    qDebug()<<"1-1 stackIndex-currentIndex="<<stackIndex-currentIndex;

    if(stackIndex-currentIndex>0)
    {
        // 上移
        qDebug()<<"1-2 up";
        dragCreat->dragJob(currentIndex,allRow,"up");
    }
    if(stackIndex-currentIndex<0)
    {
        // 下移
        qDebug()<<"1-2 down";
        dragCreat->dragJob(currentIndex,allRow,"down");
    }

    resetHash(stackIndex,currentIndex,dragCreat);
    qDebug()<<hashCreat;

}

void QJDStackWidget::resetDelHash(int current, int all,creatUI *delCreat)
{
    qDebug()<<"QJDStackWidget::resetDelHash::"<<hashCreat;
    stackIndex--;
    if(all-current==0)
    {
        hashCreat.remove(delCreat);
    }
    if(all-current>0)
    {
        hashCreat.remove(delCreat);
        creatUI *creatui;
        foreach(creatui,hashCreat.keys())
        {
            if(creatui!=delCreat && hashCreat.value(creatui)>current)
            {
                hashCreat.insert(creatui,hashCreat.value(creatui)-1);
            }
        }
    }
}

void QJDStackWidget::resetHash(int before, int after, creatUI *dragC)
{
    // down
    if(after-before>0)
    {
        creatUI *creatui;
        foreach(creatui,hashCreat.keys())
        {
            if(creatui!=dragC &&
                    hashCreat.value(creatui)<=after &&
                    hashCreat.value(creatui)>=before)
            {
                hashCreat.insert(creatui,hashCreat.value(creatui)-1);
            }
        }
        hashCreat.insert(dragC,after);
    }

    // up
    if(after-before<0)
    {
        creatUI *creatui;
        foreach(creatui,hashCreat.keys())
        {
            if(creatui!=dragC &&
                    hashCreat.value(creatui)>=after &&
                    hashCreat.value(creatui)<=before)
            {
                hashCreat.insert(creatui,hashCreat.value(creatui)+1);
            }
        }
        hashCreat.insert(dragC,after);
    }
}

void QJDStackWidget::setCurrentHashIndex(QListWidgetItem *item)
{
    /// 忠于创建时的顺序
    setCurrentIndex(hashListCreat.value(item));
}
