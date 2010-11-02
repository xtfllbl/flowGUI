#include "qjdstackwidget.h"

// 在此widget中，拜访各种参数界面
qjdStackWidget::qjdStackWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    resize(300,300);
    hashIndex.clear();
}


// 还需要函数跳转到这个页面
/// 按照名字来，使用qmap,qhash之类的东西保存名字和index的关系
void qjdStackWidget::handleMousePress(const QString name)
{
    if(name!="")
    {
        setCurrentIndex(hashIndex.value(name));
    }
    else
    {
        qDebug("isn`t a widget");
    }

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
    creat=new creatUI();
    xmlModule=new readXMLModule();  //!! 必须有一套对应，一对多是不可能正确的 !!!!!!!!

    /// ---------------传递文件名--------------------------//
    if(!jobFileName.isEmpty())
    {
        creat->setJobFileName(jobFileName);   //这个时候creat还没有创建呢
    }

    /// ----------------------------解析模块----------------------------
    /// 应当根据索引自动解析，而且是点击之后解析
    connect(xmlModule,
            SIGNAL(sigNodeData(QString,QString,QString,QString,QString,QString,QString,QStringList,QStringList,QString)),
            creat,
            SLOT(nodeData(QString,QString,QString,QString,QString,QString,QString,QStringList,QStringList,QString)));

    connect(xmlModule,SIGNAL(sigReadFinish()),creat,SLOT(finishCreatUI()));
    QFile fileXML;
    fileXML.setFileName(flowPath);
    if(!fileXML.open(QFile::ReadOnly))
        qDebug()<<"module file open failed at:"<<flowPath;

    xmlModule->read(&fileXML);
    fileXML.close();
    /// ------------------------------------------------------------------
    addWidget(creat);
    setCurrentWidget(creat);


    qDebug()<<"qjdStackWidget::creatWidget  out";
}

void qjdStackWidget::getJobXMLfileName(const QString fileName)
{
    qDebug()<<"fileName arrived~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    jobFileName=fileName;
}
