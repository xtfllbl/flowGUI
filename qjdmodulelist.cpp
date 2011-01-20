#include "qjdmodulelist.h"
#include <QDebug>
#include <QtGui>

QJDModuleList::QJDModuleList(QWidget *parent) :
    QTreeWidget(parent)
{
    setFrameShape(QFrame::WinPanel);
    setFrameShadow(QFrame::Raised);
    setHeaderHidden(true);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    indexXML=new readXMLIndex();

    analysisIndexXML();
    addModule();

    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int))
            ,this,SLOT(handleItemDoubleClickedEvent(QTreeWidgetItem*,int)));
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void QJDModuleList::analysisIndexXML()
{
    /// ----------------------------解析索引----------------------------
    fileXMLIndex.setFileName("xml/index.xml");
    fileXMLIndex.open(QFile::ReadOnly);
    indexXML->read(&fileXMLIndex);
}

void QJDModuleList::addModule()
{
    QStringList a,b,c,d,e;
    a=indexXML->getModuleName();
    b=indexXML->getModuleVersion();
    c=indexXML->getModulePath();   //路径
    d=indexXML->getModuleDesc();    // 介绍,应当怎么显示呢？传输到其他控件麻烦，暂时搁置
    e=indexXML->getModuleType();

    QTreeWidgetItem *io = new QTreeWidgetItem(this);
    io->setText(0,"Data IO");
    expandItem(io);
    QTreeWidgetItem *process = new QTreeWidgetItem(this);
    process->setText(0,"Data Process");
    expandItem(process);

    for(int i=0;i<a.size();i++)
    {
        if(e[i]=="Data IO")
        {
            int row=0;
            QTreeWidgetItem *itemIO = new QTreeWidgetItem(io);
            itemIO->setText(row,a[i]);
            row++;
        }
        if(e[i]=="Data Process")
        {
            int row=0;
            QTreeWidgetItem *itemProcess = new QTreeWidgetItem(process);
            itemProcess->setText(row,a[i]);
            row++;
        }
    }
}

void QJDModuleList::handleItemDoubleClickedEvent(QTreeWidgetItem *item, int col)
{
    qDebug()<<item->text(col);  // 不想要的text也会有。。

    QStringList a,c;
    a=indexXML->getModuleName();
    c=indexXML->getModulePath();   //路径

    QString flowName=item->text(col);
    int flowIndex=-1;
    for(int i=0;i<a.size();i++)
    {
        if(a[i]==flowName)
        {
            flowIndex=i;
        }
    }

    if(flowName=="Data IO" || flowName=="Data Process")
    {
        return;
    }
    else
    {
        // 1.还要添加到邻居的flow列表中
        emit sigAddFlow(flowName);
        // 2.双击之后，添加模块(widget),首先要生成widget，包括读取模块xml
        QString flowPath=indexXML->getModulePath().at(flowIndex);
        emit sigAddFlowList(flowName,flowPath);
    }
}
